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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/ZoneTerminalUnitList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirConditionerVariableRefrigerantFlow(AirConditionerVariableRefrigerantFlow& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = modelObject.terminals();
    if (terminals.empty()) {
      LOG(Warn, modelObject.briefDescription() << " will not be translated as it has no terminals.");
      return boost::none;
    }

    // Heat Pump Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirConditioner_VariableRefrigerantFlow, modelObject);

    // AvailabilityScheduleName

    if (boost::optional<model::Schedule> schedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // RatedTotalCoolingCapacity

    if (modelObject.isGrossRatedTotalCoolingCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, "Autosize");
    } else if ((value = modelObject.grossRatedTotalCoolingCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, value.get());
    }

    // RatedCoolingCOP

    if ((value = modelObject.grossRatedCoolingCOP())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedCoolingCOP, value.get());
    }

    // MinimumCondenserInletNodeTemperatureinCoolingMode

    if ((value = modelObject.minimumOutdoorTemperatureinCoolingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinCoolingMode, value.get());
    }

    // MaximumCondenserInletNodeTemperatureinCoolingMode

    if ((value = modelObject.maximumOutdoorTemperatureinCoolingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinCoolingMode, value.get());
    }

    // CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // CoolingCapacityRatioBoundaryCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName, _curve->name().get());
      }
    }

    // CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // CoolingEnergyInputRatioBoundaryCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName, _curve->name().get());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
                            _curve->name().get());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            _curve->name().get());
      }
    }

    // CoolingCombinationRatioCorrectionFactorCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingCombinationRatioCorrectionFactorCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName, _curve->name().get());
      }
    }

    // CoolingPartLoadFractionCorrelationCurveName

    if (boost::optional<model::Curve> curve = modelObject.coolingPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    // RatedTotalHeatingCapacity

    if (modelObject.isGrossRatedHeatingCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, "Autosize");
    } else if ((value = modelObject.grossRatedHeatingCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, value.get());
    }

    // RatedTotalHeatingCapacitySizingRatio

    if ((value = modelObject.ratedHeatingCapacitySizingRatio())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, value.get());
    }

    // RatedHeatingCOP

    if ((value = modelObject.ratedHeatingCOP())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCOP, value.get());
    }

    // MinimumCondenserInletNodeTemperatureinHeatingMode

    if ((value = modelObject.minimumOutdoorTemperatureinHeatingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinHeatingMode, value.get());
    }

    // MaximumCondenserInletNodeTemperatureinHeatingMode

    if ((value = modelObject.maximumOutdoorTemperatureinHeatingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatingMode, value.get());
    }

    // HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // HeatingCapacityRatioBoundaryCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName, _curve->name().get());
      }
    }

    // HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // HeatingEnergyInputRatioBoundaryCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName, _curve->name().get());
      }
    }

    // HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // HeatingPerformanceCurveOutdoorTemperatureType

    if ((s = modelObject.heatingPerformanceCurveOutdoorTemperatureType())) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType, s.get());
    }

    // HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
                            _curve->name().get());
      }
    }

    // HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            _curve->name().get());
      }
    }

    // HeatingCombinationRatioCorrectionFactorCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingCombinationRatioCorrectionFactorCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName, _curve->name().get());
      }
    }

    // HeatingPartLoadFractionCorrelationCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    // MinimumHeatPumpPartLoadRatio

    if ((value = modelObject.minimumHeatPumpPartLoadRatio())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, value.get());
    }

    // ZoneNameforMasterThermostatLocation

    if (boost::optional<model::ThermalZone> zone = modelObject.zoneforMasterThermostatLocation()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, zone->name().get());
    }

    // MasterThermostatPriorityControlType

    if ((s = modelObject.masterThermostatPriorityControlType())) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType, s.get());
    }

    // ThermostatPriorityScheduleName

    if (boost::optional<model::Schedule> schedule = modelObject.thermostatPrioritySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ThermostatPriorityScheduleName, _schedule->name().get());
      }
    }

    // HeatPumpWasteHeatRecovery

    if (modelObject.heatPumpWasteHeatRecovery()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, "Yes");
    } else {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, "No");
    }

    // EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode

    if ((value = modelObject.equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,
                          value.get());
    }

    // VerticalHeightusedforPipingCorrectionFactor

    if ((value = modelObject.verticalHeightusedforPipingCorrectionFactor())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor, value.get());
    }

    // PipingCorrectionFactorforLengthinCoolingModeCurveName

    if (boost::optional<model::Curve> curve = modelObject.pipingCorrectionFactorforLengthinCoolingModeCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName,
                            _curve->name().get());
      }
    }

    // PipingCorrectionFactorforHeightinCoolingModeCoefficient

    if ((value = modelObject.pipingCorrectionFactorforHeightinCoolingModeCoefficient())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient, value.get());
    }

    // EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode

    if ((value = modelObject.equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,
                          value.get());
    }

    // PipingCorrectionFactorforLengthinHeatingModeCurveName

    if (boost::optional<model::Curve> curve = modelObject.pipingCorrectionFactorforLengthinHeatingModeCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName,
                            _curve->name().get());
      }
    }

    // PipingCorrectionFactorforHeightinHeatingModeCoefficient

    if ((value = modelObject.pipingCorrectionFactorforHeightinHeatingModeCoefficient())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient, value.get());
    }

    // CrankcaseHeaterPowerperCompressor

    if ((value = modelObject.crankcaseHeaterPowerperCompressor())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor, value.get());
    }

    // NumberofCompressors

    {
      int number = modelObject.numberofCompressors();

      idfObject.setUnsigned(AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors, (unsigned)number);
    }

    // RatioofCompressorSizetoTotalCompressorCapacity

    if ((value = modelObject.ratioofCompressorSizetoTotalCompressorCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity, value.get());
    }

    // MaximumOutdoorDrybulbTemperatureforCrankcaseHeater

    if ((value = modelObject.maximumOutdoorDrybulbTemperatureforCrankcaseHeater())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, value.get());
    }

    // DefrostStrategy

    if ((s = modelObject.defrostStrategy())) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, s.get());
    }

    // DefrostControl

    if ((s = modelObject.defrostControl())) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostControl, s.get());
    }

    // DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.defrostEnergyInputRatioModifierFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // DefrostTimePeriodFraction

    if ((value = modelObject.defrostTimePeriodFraction())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction, value.get());
    }

    // ResistiveDefrostHeaterCapacity

    if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, "Autosize");
    } else if ((value = modelObject.resistiveDefrostHeaterCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, value.get());
    }

    // MaximumOutdoorDrybulbTemperatureforDefrostOperation

    if ((value = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, value.get());
    }

    // Condenser Type
    // It was decided that the "smart" logic shouldn't be in the model itself.
    // So now, we do two things:
    // * If condenserType is set, respect that, but issue Errors if it's wrong
    // * If condenserType is not set, we default it (default is done in the model actually)
    //     * If VRF connected to a PlantLoop => WaterCooled, else "AirCooled" ("EvaporativelyCooled" is less common and will be reserved for people who
    //     know what they are doing and are hardsetting it)
    std::string condenserType = modelObject.condenserType();

    if (modelObject.isCondenserTypeDefaulted()) {
      // We log an Info anyways, might be useful
      if (openstudio::istringEqual(condenserType, "EvaporativelyCooled")) {
        LOG(Info, modelObject.briefDescription() << " is connected to a PlantLoop, defaulting condenserType to 'WaterCooled'.");
      } else {
        LOG(Info, modelObject.briefDescription() << " is not connected to a PlantLoop, defaulting condenserType to 'AirCooled'.");
      }
    } else {
      boost::optional<PlantLoop> _plant = modelObject.plantLoop();

      if ((openstudio::istringEqual(condenserType, "AirCooled") || openstudio::istringEqual(condenserType, "EvaporativelyCooled"))
          && _plant.is_initialized()) {
        LOG(Error,
            modelObject.briefDescription() << " has an hardcoded condenserType '" << condenserType << "' while it is connected to a PlantLoop.");
      } else if (openstudio::istringEqual(condenserType, "WaterCooled") && !_plant.is_initialized()) {
        LOG(Error,
            modelObject.briefDescription() << " has an hardcoded condenserType '" << condenserType << "' while it is NOT connected to a PlantLoop.");
      }
    }
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserType, condenserType);

    // CondenserInletNodeName
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserInletNodeName, node->name().get());
      }
    }
    // CondenserOutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNodeName, node->name().get());
      }
    }

    // WaterCondenserVolumeFlowRate

    if (modelObject.isWaterCondenserVolumeFlowRateAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, "Autosize");
    } else if ((value = modelObject.waterCondenserVolumeFlowRate())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, value.get());
    }

    // EvaporativeCondenserEffectiveness

    if ((value = modelObject.evaporativeCondenserEffectiveness())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness, value.get());
    }

    // EvaporativeCondenserAirFlowRate

    if (modelObject.isEvaporativeCondenserAirFlowRateAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, "Autosize");
    } else if ((value = modelObject.evaporativeCondenserAirFlowRate())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, value.get());
    }

    // EvaporativeCondenserPumpRatedPowerConsumption

    if (modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    } else if ((value = modelObject.evaporativeCondenserPumpRatedPowerConsumption())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, value.get());
    }

    // BasinHeaterCapacity

    if ((value = modelObject.basinHeaterCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity, value.get());
    }

    // BasinHeaterSetpointTemperature

    if ((value = modelObject.basinHeaterSetpointTemperature())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature, value.get());
    }

    // BasinHeaterOperatingScheduleName

    if (boost::optional<model::Schedule> schedule = modelObject.basinHeaterOperatingSchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingScheduleName, _schedule->name().get());
      }
    }

    // FuelType

    if ((s = modelObject.fuelType())) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::FuelType, s.get());
    }

    // MinimumCondenserInletNodeTemperatureinHeatRecoveryMode

    if ((value = modelObject.minimumOutdoorTemperatureinHeatRecoveryMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinHeatRecoveryMode, value.get());
    }

    // MaximumCondenserInletNodeTemperatureinHeatRecoveryMode

    if ((value = modelObject.maximumOutdoorTemperatureinHeatingMode())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatRecoveryMode, value.get());
    }

    // HeatRecoveryCoolingCapacityModifierCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryCoolingCapacityModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName, _curve->name().get());
      }
    }

    // InitialHeatRecoveryCoolingCapacityFraction

    if ((value = modelObject.initialHeatRecoveryCoolingEnergyFraction())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction, value.get());
    }

    // HeatRecoveryCoolingCapacityTimeConstant

    if ((value = modelObject.heatRecoveryCoolingCapacityTimeConstant())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant, value.get());
    }

    // HeatRecoveryCoolingEnergyModifierCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryCoolingEnergyModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName, _curve->name().get());
      }
    }

    // InitialHeatRecoveryCoolingEnergyFraction

    if ((value = modelObject.initialHeatRecoveryCoolingEnergyFraction())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction, value.get());
    }

    // HeatRecoveryCoolingEnergyTimeConstant

    if ((value = modelObject.heatRecoveryCoolingEnergyTimeConstant())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant, value.get());
    }

    // HeatRecoveryHeatingCapacityModifierCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryHeatingCapacityModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName, _curve->name().get());
      }
    }

    // InitialHeatRecoveryHeatingCapacityFraction

    if ((value = modelObject.initialHeatRecoveryHeatingCapacityFraction())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction, value.get());
    }

    // HeatRecoveryHeatingCapacityTimeConstant

    if ((value = modelObject.heatRecoveryHeatingCapacityTimeConstant())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant, value.get());
    }

    // HeatRecoveryHeatingEnergyModifierCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryHeatingEnergyModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName, _curve->name().get());
      }
    }

    // InitialHeatRecoveryHeatingEnergyFraction

    if ((value = modelObject.initialHeatRecoveryHeatingEnergyFraction())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction, value.get());
    }

    // HeatRecoveryHeatingEnergyTimeConstant

    if ((value = modelObject.heatRecoveryHeatingEnergyTimeConstant())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant, value.get());
    }

    // Terminal Unit List

    IdfObject _zoneTerminalUnitList(IddObjectType::ZoneTerminalUnitList);

    std::string terminalUnitListName = modelObject.name().get() + " Terminal List";

    _zoneTerminalUnitList.setString(ZoneTerminalUnitListFields::ZoneTerminalUnitListName, terminalUnitListName);

    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitListName, terminalUnitListName);

    m_idfObjects.push_back(_zoneTerminalUnitList);

    for (auto& terminal : terminals) {
      boost::optional<IdfObject> _terminal = translateAndMapModelObject(terminal);

      OS_ASSERT(_terminal);

      IdfExtensibleGroup eg = _zoneTerminalUnitList.pushExtensibleGroup();

      eg.setString(ZoneTerminalUnitListExtensibleFields::ZoneTerminalUnitName, _terminal->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
