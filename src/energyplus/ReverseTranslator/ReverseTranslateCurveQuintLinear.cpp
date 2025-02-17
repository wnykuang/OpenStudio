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

#include "../ReverseTranslator.hpp"

#include "../../model/CurveQuintLinear.hpp"

#include <utilities/idd/Curve_QuintLinear_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveQuintLinear(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::CurveQuintLinear modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Coefficient1 Constant: Required Double
    if (boost::optional<double> _coefficient1Constant = workspaceObject.getDouble(Curve_QuintLinearFields::Coefficient1Constant)) {
      modelObject.setCoefficient1Constant(_coefficient1Constant.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient1 Constant'");
      return result;
    }

    // Coefficient2 v: Required Double
    if (boost::optional<double> _coefficient2v = workspaceObject.getDouble(Curve_QuintLinearFields::Coefficient2v)) {
      modelObject.setCoefficient2v(_coefficient2v.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient2 v'");
      return result;
    }

    // Coefficient3 w: Required Double
    if (boost::optional<double> _coefficient3w = workspaceObject.getDouble(Curve_QuintLinearFields::Coefficient3w)) {
      modelObject.setCoefficient3w(_coefficient3w.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient3 w'");
      return result;
    }

    // Coefficient4 x: Required Double
    if (boost::optional<double> _coefficient4x = workspaceObject.getDouble(Curve_QuintLinearFields::Coefficient4x)) {
      modelObject.setCoefficient4x(_coefficient4x.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient4 x'");
      return result;
    }

    // Coefficient5 y: Required Double
    if (boost::optional<double> _coefficient5y = workspaceObject.getDouble(Curve_QuintLinearFields::Coefficient5y)) {
      modelObject.setCoefficient5y(_coefficient5y.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient5 y'");
      return result;
    }

    // Coefficient6 z: Required Double
    if (boost::optional<double> _coefficient6z = workspaceObject.getDouble(Curve_QuintLinearFields::Coefficient6z)) {
      modelObject.setCoefficient6z(_coefficient6z.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient6 z'");
      return result;
    }

    // Minimum Value of v: Required Double
    if (boost::optional<double> _minimumValueofv = workspaceObject.getDouble(Curve_QuintLinearFields::MinimumValueofv)) {
      modelObject.setMinimumValueofv(_minimumValueofv.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of v'");
      return result;
    }

    // Maximum Value of v: Required Double
    if (boost::optional<double> _maximumValueofv = workspaceObject.getDouble(Curve_QuintLinearFields::MaximumValueofv)) {
      modelObject.setMaximumValueofv(_maximumValueofv.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of v'");
      return result;
    }

    // Minimum Value of w: Required Double
    if (boost::optional<double> _minimumValueofw = workspaceObject.getDouble(Curve_QuintLinearFields::MinimumValueofw)) {
      modelObject.setMinimumValueofw(_minimumValueofw.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of w'");
      return result;
    }

    // Maximum Value of w: Required Double
    if (boost::optional<double> _maximumValueofw = workspaceObject.getDouble(Curve_QuintLinearFields::MaximumValueofw)) {
      modelObject.setMaximumValueofw(_maximumValueofw.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of w'");
      return result;
    }

    // Minimum Value of x: Required Double
    if (boost::optional<double> _minimumValueofx = workspaceObject.getDouble(Curve_QuintLinearFields::MinimumValueofx)) {
      modelObject.setMinimumValueofx(_minimumValueofx.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of x'");
      return result;
    }

    // Maximum Value of x: Required Double
    if (boost::optional<double> _maximumValueofx = workspaceObject.getDouble(Curve_QuintLinearFields::MaximumValueofx)) {
      modelObject.setMaximumValueofx(_maximumValueofx.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of x'");
      return result;
    }

    // Minimum Value of y: Required Double
    if (boost::optional<double> _minimumValueofy = workspaceObject.getDouble(Curve_QuintLinearFields::MinimumValueofy)) {
      modelObject.setMinimumValueofy(_minimumValueofy.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of y'");
      return result;
    }

    // Maximum Value of y: Required Double
    if (boost::optional<double> _maximumValueofy = workspaceObject.getDouble(Curve_QuintLinearFields::MaximumValueofy)) {
      modelObject.setMaximumValueofy(_maximumValueofy.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of y'");
      return result;
    }

    // Minimum Value of z: Required Double
    if (boost::optional<double> _minimumValueofz = workspaceObject.getDouble(Curve_QuintLinearFields::MinimumValueofz)) {
      modelObject.setMinimumValueofz(_minimumValueofz.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of z'");
      return result;
    }

    // Maximum Value of z: Required Double
    if (boost::optional<double> _maximumValueofz = workspaceObject.getDouble(Curve_QuintLinearFields::MaximumValueofz)) {
      modelObject.setMaximumValueofz(_maximumValueofz.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of z'");
      return result;
    }

    // Minimum Curve Output: Optional Double
    if (boost::optional<double> _minimumCurveOutput = workspaceObject.getDouble(Curve_QuintLinearFields::MinimumCurveOutput)) {
      modelObject.setMinimumCurveOutput(_minimumCurveOutput.get());
    }

    // Maximum Curve Output: Optional Double
    if (boost::optional<double> _maximumCurveOutput = workspaceObject.getDouble(Curve_QuintLinearFields::MaximumCurveOutput)) {
      modelObject.setMaximumCurveOutput(_maximumCurveOutput.get());
    }

    // Input Unit Type for v: Optional String
    if (boost::optional<std::string> _inputUnitTypeforv = workspaceObject.getString(Curve_QuintLinearFields::InputUnitTypeforv)) {
      modelObject.setInputUnitTypeforv(_inputUnitTypeforv.get());
    }

    // Input Unit Type for w: Optional String
    if (boost::optional<std::string> _inputUnitTypeforw = workspaceObject.getString(Curve_QuintLinearFields::InputUnitTypeforw)) {
      modelObject.setInputUnitTypeforw(_inputUnitTypeforw.get());
    }

    // Input Unit Type for x: Optional String
    if (boost::optional<std::string> _inputUnitTypeforx = workspaceObject.getString(Curve_QuintLinearFields::InputUnitTypeforx)) {
      modelObject.setInputUnitTypeforx(_inputUnitTypeforx.get());
    }

    // Input Unit Type for y: Optional String
    if (boost::optional<std::string> _inputUnitTypefory = workspaceObject.getString(Curve_QuintLinearFields::InputUnitTypefory)) {
      modelObject.setInputUnitTypefory(_inputUnitTypefory.get());
    }

    // Input Unit Type for z: Optional String
    if (boost::optional<std::string> _inputUnitTypeforz = workspaceObject.getString(Curve_QuintLinearFields::InputUnitTypeforz)) {
      modelObject.setInputUnitTypeforz(_inputUnitTypeforz.get());
    }

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
