/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurfaceGroup_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_InteriorPartitionSurface) {
  Model model;
  Point3dVector points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 1, model);
  boost::optional<Space> space2 = Space::fromFloorPrint(points, 1, model);

  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);
  space2->setXOrigin(1);

  StandardOpaqueMaterial material(model);

  std::vector<Material> layers;
  layers.push_back(material);

  Construction construction(model);
  EXPECT_TRUE(construction.setLayers(layers));

  // interior walls will be converted to internal mass
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setWallConstruction(construction);

  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultInteriorSurfaceConstructions(defaultSurfaceConstructions);

  space1->setDefaultConstructionSet(defaultConstructionSet);
  space2->setDefaultConstructionSet(defaultConstructionSet);

  ThermalZone zone(model);
  space1->setThermalZone(zone);
  space2->setThermalZone(zone);
  space1->matchSurfaces(*space2);

  EXPECT_EQ(12u, model.getConcreteModelObjects<Surface>().size());

  unsigned n = 0;
  for (const Surface& surface : space1->surfaces()) {
    EXPECT_EQ(1.0, surface.grossArea());
    if (surface.adjacentSurface()) {
      ++n;
    }
  }
  EXPECT_EQ(1u, n);

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    // In this case, the surface that is between the two Spaces will be converted to an InternalMass object
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(10u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::InternalMass).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());

    WorkspaceObject internalMassObject = workspace.getObjectsByType(IddObjectType::InternalMass)[0];
    ASSERT_TRUE(internalMassObject.getDouble(InternalMassFields::SurfaceArea));
    EXPECT_EQ(1.0, internalMassObject.getDouble(InternalMassFields::SurfaceArea).get());

    WorkspaceObject constructionObject = workspace.getObjectsByType(IddObjectType::Construction)[0];
    ASSERT_TRUE(internalMassObject.getTarget(InternalMassFields::ConstructionName));
    EXPECT_EQ(constructionObject.handle(), internalMassObject.getTarget(InternalMassFields::ConstructionName)->handle());

    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
    ASSERT_TRUE(internalMassObject.getTarget(InternalMassFields::ZoneorZoneListName));
    EXPECT_EQ(zoneObject.handle(), internalMassObject.getTarget(InternalMassFields::ZoneorZoneListName)->handle());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    // In this case, the surface between the two spaces does exist. Each space is its own enclosure
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(12u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::InternalMass).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());
  }
}
