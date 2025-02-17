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
#include "ModelFixture.hpp"

#include "../ZoneAirMassFlowConservation.hpp"
#include "../ZoneAirMassFlowConservation_Impl.hpp"
#include "../Model.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneAirMassFlowConservation) {
  Model model;

  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());

  auto zamfc = model.getUniqueModelObject<ZoneAirMassFlowConservation>();

  EXPECT_EQ("None", zamfc.adjustZoneMixingandReturnForAirMassFlowBalance());
  EXPECT_TRUE(zamfc.isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted());

  // deprecated
  EXPECT_FALSE(zamfc.adjustZoneMixingForZoneAirMassFlowBalance());
  EXPECT_TRUE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());

  EXPECT_EQ("AddInfiltrationFlow", zamfc.infiltrationBalancingMethod());
  EXPECT_TRUE(zamfc.isInfiltrationBalancingMethodDefaulted());

  EXPECT_EQ("MixingSourceZonesOnly", zamfc.infiltrationBalancingZones());
  EXPECT_TRUE(zamfc.isInfiltrationBalancingZonesDefaulted());

  EXPECT_TRUE(zamfc.setAdjustZoneMixingandReturnForAirMassFlowBalance("AdjustReturnThenMixing"));
  EXPECT_EQ("AdjustReturnThenMixing", zamfc.adjustZoneMixingandReturnForAirMassFlowBalance());
  EXPECT_FALSE(zamfc.isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted());

  // deprecated
  EXPECT_FALSE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());
  zamfc.setAdjustZoneMixingForZoneAirMassFlowBalance(true);
  EXPECT_EQ("AdjustMixingOnly", zamfc.adjustZoneMixingandReturnForAirMassFlowBalance());
  EXPECT_TRUE(zamfc.adjustZoneMixingForZoneAirMassFlowBalance());
  EXPECT_FALSE(zamfc.isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted());

  EXPECT_TRUE(zamfc.setInfiltrationBalancingMethod("AdjustInfiltrationFlow"));
  EXPECT_EQ("AdjustInfiltrationFlow", zamfc.infiltrationBalancingMethod());
  EXPECT_FALSE(zamfc.isInfiltrationBalancingMethodDefaulted());

  EXPECT_TRUE(zamfc.setInfiltrationBalancingZones("AllZones"));
  EXPECT_EQ("AllZones", zamfc.infiltrationBalancingZones());
  EXPECT_FALSE(zamfc.isInfiltrationBalancingZonesDefaulted());

  EXPECT_TRUE(model.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
}
