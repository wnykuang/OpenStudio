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

#include "CoilCoolingDXSingleSpeed.hpp"
#include "CoilCoolingDXSingleSpeed_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    // constructor
    CoilCoolingDXSingleSpeed_Impl::CoilCoolingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXSingleSpeed::iddObjectType());
    }

    CoilCoolingDXSingleSpeed_Impl::CoilCoolingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXSingleSpeed::iddObjectType());
    }

    CoilCoolingDXSingleSpeed_Impl::CoilCoolingDXSingleSpeed_Impl(const CoilCoolingDXSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    ModelObject CoilCoolingDXSingleSpeed_Impl::clone(Model model) const {
      auto newCoil = StraightComponent_Impl::clone(model).cast<CoilCoolingDXSingleSpeed>();

      return std::move(newCoil);
    }

    std::vector<IddObjectType> CoilCoolingDXSingleSpeed_Impl::allowableChildTypes() const {
      std::vector<IddObjectType> result;
      result.push_back(IddObjectType::OS_Curve_Biquadratic);
      result.push_back(IddObjectType::OS_Curve_Quadratic);
      result.push_back(IddObjectType::OS_Curve_Cubic);
      return result;
    }

    std::vector<ModelObject> CoilCoolingDXSingleSpeed_Impl::children() const {

      std::vector<ModelObject> result;
      if (OptionalCurve intermediate = optionalTotalCoolingCapacityFunctionOfTemperatureCurve()) {
        result.push_back(*intermediate);
      }
      if (OptionalCurve intermediate = optionalTotalCoolingCapacityFunctionOfFlowFractionCurve()) {
        result.push_back(*intermediate);
      }
      if (OptionalCurve intermediate = optionalEnergyInputRatioFunctionOfTemperatureCurve()) {
        result.push_back(*intermediate);
      }
      if (OptionalCurve intermediate = optionalEnergyInputRatioFunctionOfFlowFractionCurve()) {
        result.push_back(*intermediate);
      }
      if (OptionalCurve intermediate = optionalPartLoadFractionCorrelationCurve()) {
        result.push_back(*intermediate);
      }
      return result;
    }
    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& CoilCoolingDXSingleSpeed_Impl::outputVariableNames() const {
      // TODO: static for now
      static const std::vector<std::string> result{
        "Cooling Coil Total Cooling Rate", "Cooling Coil Total Cooling Energy", "Cooling Coil Sensible Cooling Rate",
        "Cooling Coil Sensible Cooling Energy", "Cooling Coil Latent Cooling Rate", "Cooling Coil Latent Cooling Energy",
        "Cooling Coil Electricity Rate", "Cooling Coil Electricity Energy", "Cooling Coil Runtime Fraction",

        // condenserType = [AirCooled, EvaporativelyCooled]
        // if (this->condenserType() == "EvaporativelyCooled") {
        "Cooling Coil Condenser Inlet Temperature", "Cooling Coil Evaporative Condenser Water Volume",
        "Cooling Coil Evaporative Condenser Pump Electricity Rate", "Cooling Coil Evaporative Condenser Pump Electricity Energy",
        "Cooling Coil Basin Heater Electricity Rate", "Cooling Coil Basin Heater Electricity Energy",
        "Cooling Coil Evaporative Condenser Mains Supply Water Volume"
        // }

        // Storage tank isn't implemented
        // if has storage tank:
        // "Cooling Coil Condensate Volume Flow Rate",
        // "Cooling Coil Condensate Volume",
        //

        // If not part of AirLoopHVAC:UnitaryHeatPump:AirToAir
        // (if part of a heat pump, crankcase heater is reported only for the heating coil):
        // if ( !this->containingHVACComponent().empty() ) {
        // "Cooling Coil Crankcase Heater Electricity Rate",
        // "Cooling Coil Crankcase Heater Electricity Energy",
        // }

      };
      return result;
    }

    IddObjectType CoilCoolingDXSingleSpeed_Impl::iddObjectType() const {
      return CoilCoolingDXSingleSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDXSingleSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXSingleSpeed", "Availability"));
      }
      if (std::find(b, e, OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CoilCoolingDXSingleSpeed", "Basin Heater Operation"));
      }
      return result;
    }

    unsigned CoilCoolingDXSingleSpeed_Impl::inletPort() const {
      return OS_Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName;
    }

    unsigned CoilCoolingDXSingleSpeed_Impl::outletPort() const {
      return OS_Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName;
    }

    Schedule CoilCoolingDXSingleSpeed_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<CoilCoolingDXSingleSpeed_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName, "CoilCoolingDXSingleSpeed", "Availability", schedule);
      return result;
    }

    double CoilCoolingDXSingleSpeed_Impl::ratedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedCOP);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedCOP(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedCOP, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedCOP(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedCOP, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate2017() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeed_Impl::ratedEvaporatorFanPowerPerVolumeFlowRate2023() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate2017(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017, value);
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate2023(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023, value);
    }

    Curve CoilCoolingDXSingleSpeed_Impl::totalCoolingCapacityFunctionOfTemperatureCurve() const {
      OptionalCurve result = optionalTotalCoolingCapacityFunctionOfTemperatureCurve();
      OS_ASSERT(result);
      return result.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve) {
      return setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName, curve.handle());
    }

    Curve CoilCoolingDXSingleSpeed_Impl::totalCoolingCapacityFunctionOfFlowFractionCurve() const {
      OptionalCurve result = optionalTotalCoolingCapacityFunctionOfFlowFractionCurve();
      OS_ASSERT(result);
      return result.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve) {
      return setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, curve.handle());
    }

    Curve CoilCoolingDXSingleSpeed_Impl::energyInputRatioFunctionOfTemperatureCurve() const {
      OptionalCurve result = optionalEnergyInputRatioFunctionOfTemperatureCurve();
      OS_ASSERT(result);
      return result.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve) {
      return setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, curve.handle());
    }

    Curve CoilCoolingDXSingleSpeed_Impl::energyInputRatioFunctionOfFlowFractionCurve() const {
      OptionalCurve result = optionalEnergyInputRatioFunctionOfFlowFractionCurve();
      OS_ASSERT(result);
      return result.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve) {
      return setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, curve.handle());
    }

    Curve CoilCoolingDXSingleSpeed_Impl::partLoadFractionCorrelationCurve() const {
      OptionalCurve result = optionalPartLoadFractionCorrelationCurve();
      OS_ASSERT(result);
      return result.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setPartLoadFractionCorrelationCurve(const Curve& curve) {
      return setPointer(OS_Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName, curve.handle());
    }

    double CoilCoolingDXSingleSpeed_Impl::nominalTimeForCondensateRemovalToBegin() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setNominalTimeForCondensateRemovalToBegin(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setNominalTimeForCondensateRemovalToBegin(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::maximumCyclingRate() {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setMaximumCyclingRate(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setMaximumCyclingRate(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::latentCapacityTimeConstant() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setLatentCapacityTimeConstant(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setLatentCapacityTimeConstant(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant, value);
    }

    OptionalString CoilCoolingDXSingleSpeed_Impl::condenserAirInletNodeName() const {
      return getString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName);
    }

    bool CoilCoolingDXSingleSpeed_Impl::setCondenserAirInletNodeName(const boost::optional<std::string>& value) {
      return setString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName, value.get());
    }

    std::string CoilCoolingDXSingleSpeed_Impl::condenserType() const {
      return getString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserType).get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setCondenserType(const std::string& value) {
      return setString(OS_Coil_Cooling_DX_SingleSpeedFields::CondenserType, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::evaporativeCondenserEffectiveness() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserEffectiveness(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserEffectiveness(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness, value);
    }

    OptionalDouble CoilCoolingDXSingleSpeed_Impl::evaporativeCondenserAirFlowRate() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate);
    }

    bool CoilCoolingDXSingleSpeed_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserAirFlowRate(OptionalDouble value) {
      if (value) {
        return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, *value);
      } else {
        return setString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, "Autosize");
      }
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserAirFlowRate(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, value);
    }

    void CoilCoolingDXSingleSpeed_Impl::autosizeEvaporativeCondenserAirFlowRate() {
      bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    OptionalDouble CoilCoolingDXSingleSpeed_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption);
    }

    bool CoilCoolingDXSingleSpeed_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(OptionalDouble value) {
      if (value) {
        return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, *value);
      } else {
        return setString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
      }
    }

    bool CoilCoolingDXSingleSpeed_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, value);
    }

    void CoilCoolingDXSingleSpeed_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
      bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    double CoilCoolingDXSingleSpeed_Impl::crankcaseHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setCrankcaseHeaterCapacity(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setCrankcaseHeaterCapacity(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, value);
    }

    //getSupplyWaterStorageTankName
    //setSupplyWaterStorageTankName

    //getCondensateCollectionWaterStorageTankName
    //setCondensateCollectionWaterStorageTankName

    double CoilCoolingDXSingleSpeed_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterCapacity(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterCapacity(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity, value);
    }

    double CoilCoolingDXSingleSpeed_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterSetpointTemperature(boost::optional<double> value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature, value.get());
    }

    bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterSetpointTemperature(double value) {
      return setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature, value);
    }

    boost::optional<Schedule> CoilCoolingDXSingleSpeed_Impl::basinHeaterOperatingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName);
    }

    bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName, "CoilCoolingDXSingleSpeed",
                                "Basin Heater Operation", schedule);
      return result;
    }

    void CoilCoolingDXSingleSpeed_Impl::resetBasinHeaterOperatingSchedule() {
      bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<HVACComponent> CoilCoolingDXSingleSpeed_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
      std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

      for (const auto& bypassSystem : bypassSystems) {
        if (boost::optional<HVACComponent> coolingCoil = bypassSystem.coolingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return bypassSystem;
          }
        }
      }

      // AirLoopHVACUnitaryHeatPumpAirToAir

      std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

      airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

      for (const auto& airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs) {
        if (boost::optional<HVACComponent> coil = airLoopHVACUnitaryHeatPumpAirToAir.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return airLoopHVACUnitaryHeatPumpAirToAir;
          }
        }
      }

      // CoilSystemCoolingDXHeatExchangerAssisted
      {
        auto coilSystems = this->model().getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.coolingCoil().handle() == this->handle()) {
            return coilSystem;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> CoilCoolingDXSingleSpeed_Impl::containingZoneHVACComponent() const {
      // ZoneHVACPackagedTerminalAirConditioner

      std::vector<ZoneHVACPackagedTerminalAirConditioner> zoneHVACPackagedTerminalAirConditioners;

      zoneHVACPackagedTerminalAirConditioners = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalAirConditioner>();

      for (const auto& zoneHVACPackagedTerminalAirConditioner : zoneHVACPackagedTerminalAirConditioners) {
        if (boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalAirConditioner.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACPackagedTerminalAirConditioner;
          }
        }
      }

      // ZoneHVACPackagedTerminalHeatPump

      std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

      zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

      for (const auto& zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps) {
        if (boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.coolingCoil()) {
          if (coil->handle() == this->handle()) {
            return zoneHVACPackagedTerminalHeatPump;
          }
        }
      }

      return boost::none;
    }

    boost::optional<Schedule> CoilCoolingDXSingleSpeed_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName);
    }
    boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
    }
    boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName);
    }
    boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalEnergyInputRatioFunctionOfTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName);
    }
    boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalEnergyInputRatioFunctionOfFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName);
    }
    boost::optional<Curve> CoilCoolingDXSingleSpeed_Impl::optionalPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName);
    }

    boost::optional<ModelObject> CoilCoolingDXSingleSpeed_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    boost::optional<ModelObject> CoilCoolingDXSingleSpeed_Impl::basinHeaterOperatingScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = basinHeaterOperatingSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setBasinHeaterOperatingSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetBasinHeaterOperatingSchedule();
      }
      return true;
    }

    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::ratedTotalCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeed_Impl::isRatedTotalCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity) {
      bool result(false);
      if (ratedTotalCoolingCapacity) {
        result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, ratedTotalCoolingCapacity.get());
      }
      return result;
    }

    void CoilCoolingDXSingleSpeed_Impl::autosizeRatedTotalCoolingCapacity() {
      bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedTotalCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::ratedSensibleHeatRatio() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, true);
    }

    bool CoilCoolingDXSingleSpeed_Impl::isRatedSensibleHeatRatioAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedSensibleHeatRatio(boost::optional<double> ratedSensibleHeatRatio) {
      bool result(false);
      if (ratedSensibleHeatRatio) {
        result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, ratedSensibleHeatRatio.get());
      }
      return result;
    }

    void CoilCoolingDXSingleSpeed_Impl::autosizeRatedSensibleHeatRatio() {
      bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedSensibleHeatRatio, "autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::ratedAirFlowRate() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, true);
    }

    bool CoilCoolingDXSingleSpeed_Impl::isRatedAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
      bool result(false);
      if (ratedAirFlowRate) {
        result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, ratedAirFlowRate.get());
      }
      return result;
    }

    void CoilCoolingDXSingleSpeed_Impl::autosizeRatedAirFlowRate() {
      bool result = setString(OS_Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    double CoilCoolingDXSingleSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool CoilCoolingDXSingleSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
      bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                              minimumOutdoorDryBulbTemperatureforCompressorOperation);
      return result;
    }

    bool CoilCoolingDXSingleSpeed_Impl::addToNode(Node& node) {
      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (!airLoop->demandComponent(node.handle())) {
          // TODO: JM 2019-03-12 I'm not sure we shouldn't just restrict to ANY containingHVACComponent (disallow if part of a UnitarySystem)
          auto t_containingHVACComponent = containingHVACComponent();
          if (t_containingHVACComponent && t_containingHVACComponent->optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
            LOG(Warn,
                this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingDXHeatExchangerAssisted. "
                                            "Please call CoilSystemCoolingDXHeatExchangerAssisted::addToNode instead");
          } else {
            return StraightComponent_Impl::addToNode(node);
          }
        }
      } else if (boost::optional<AirLoopHVACOutdoorAirSystem> oas = node.airLoopHVACOutdoorAirSystem()) {
        if (oas->airLoopHVACDedicatedOutdoorAirSystem()) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    AirflowNetworkEquivalentDuct CoilCoolingDXSingleSpeed_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
      boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
      if (opt) {
        if (opt->airPathLength() != length) {
          opt->setAirPathLength(length);
        }
        if (opt->airPathHydraulicDiameter() != diameter) {
          opt->setAirPathHydraulicDiameter(diameter);
        }
      }
      return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
    }

    boost::optional<AirflowNetworkEquivalentDuct> CoilCoolingDXSingleSpeed_Impl::airflowNetworkEquivalentDuct() const {
      std::vector<AirflowNetworkEquivalentDuct> myAFN =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      auto count = myAFN.size();
      if (count == 1) {
        return myAFN[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
        return myAFN[0];
      }
      return boost::none;
    }

    // Autosizing methods
    void CoilCoolingDXSingleSpeed_Impl::autosize() {
      autosizeRatedTotalCoolingCapacity();
      autosizeRatedSensibleHeatRatio();
      autosizeRatedAirFlowRate();
      autosizeEvaporativeCondenserAirFlowRate();
      autosizeEvaporativeCondenserPumpRatedPowerConsumption();
    }

    void CoilCoolingDXSingleSpeed_Impl::applySizingValues() {
      boost::optional<double> val;

      val = autosizedRatedAirFlowRate();
      if (val) {
        setRatedAirFlowRate(val.get());
      }

      val = autosizedRatedTotalCoolingCapacity();
      if (val) {
        setRatedTotalCoolingCapacity(val.get());
      }

      val = autosizedRatedSensibleHeatRatio();
      if (val) {
        setRatedSensibleHeatRatio(val.get());
      }

      val = autosizedEvaporativeCondenserAirFlowRate();
      if (val) {
        setEvaporativeCondenserAirFlowRate(val.get());
      }

      val = autosizedEvaporativeCondenserPumpRatedPowerConsumption();
      if (val) {
        setEvaporativeCondenserPumpRatedPowerConsumption(val.get());
      }
    }

    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::autosizedRatedAirFlowRate() const {
      return getAutosizedValue("Design Size Rated Air Flow Rate", "m3/s");
    }

    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::autosizedRatedTotalCoolingCapacity() const {
      return getAutosizedValue("Design Size Gross Rated Total Cooling Capacity", "W");
    }

    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::autosizedRatedSensibleHeatRatio() const {
      return getAutosizedValue("Design Size Gross Rated Sensible Heat Ratio", "");
    }

    // E+ output has a bug where this field label is incorrect
    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::autosizedEvaporativeCondenserAirFlowRate() const {
      return getAutosizedValue("Design Size Evaporative Condenser Air Flow Rate", "m3/s");
    }

    // E+ output has a bug where this field label is incorrect
    boost::optional<double> CoilCoolingDXSingleSpeed_Impl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
      return getAutosizedValue("Design Size Evaporative Condenser Pump Rated Power Consumption", "W");
    }

    std::vector<EMSActuatorNames> CoilCoolingDXSingleSpeed_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Coil:Cooling:DX:SingleSpeed", "Autosized Rated Air Flow Rate"},
                                              {"Coil:Cooling:DX:SingleSpeed", "Autosized Rated Sensible Heat Ratio"},
                                              {"Coil:Cooling:DX:SingleSpeed", "Autosized Rated Total Cooling Capacity"},
                                              {"Outdoor Air System Node", "Drybulb Temperature"},
                                              {"Outdoor Air System Node", "Wetbulb Temperature"},
                                              {"Outdoor Air System Node", "Wind Speed"},
                                              {"Outdoor Air System Node", "Wind Direction"}};
      return actuators;
    }

    std::vector<std::string> CoilCoolingDXSingleSpeed_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    ComponentType CoilCoolingDXSingleSpeed_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilCoolingDXSingleSpeed_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> CoilCoolingDXSingleSpeed_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingDXSingleSpeed_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  CoilCoolingDXSingleSpeed::CoilCoolingDXSingleSpeed(const Model& model) : StraightComponent(CoilCoolingDXSingleSpeed::iddObjectType(), model) {
    model::CurveBiquadratic coolingCurveFofTemp = CurveBiquadratic(model);
    coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
    coolingCurveFofTemp.setCoefficient2x(0.009543347);
    coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
    coolingCurveFofTemp.setCoefficient4y(-0.011042676);
    coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
    coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
    coolingCurveFofTemp.setMinimumValueofx(17.0);
    coolingCurveFofTemp.setMaximumValueofx(22.0);
    coolingCurveFofTemp.setMinimumValueofy(13.0);
    coolingCurveFofTemp.setMaximumValueofy(46.0);

    CurveQuadratic coolingCurveFofFlow = CurveQuadratic(model);
    coolingCurveFofFlow.setCoefficient1Constant(0.8);
    coolingCurveFofFlow.setCoefficient2x(0.2);
    coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
    coolingCurveFofFlow.setMinimumValueofx(0.5);
    coolingCurveFofFlow.setMaximumValueofx(1.5);

    CurveBiquadratic energyInputRatioFofTemp = CurveBiquadratic(model);
    energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
    energyInputRatioFofTemp.setCoefficient2x(0.034885008);
    energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
    energyInputRatioFofTemp.setCoefficient4y(0.004977216);
    energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
    energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
    energyInputRatioFofTemp.setMinimumValueofx(17.0);
    energyInputRatioFofTemp.setMaximumValueofx(22.0);
    energyInputRatioFofTemp.setMinimumValueofy(13.0);
    energyInputRatioFofTemp.setMaximumValueofy(46.0);

    CurveQuadratic energyInputRatioFofFlow = CurveQuadratic(model);
    energyInputRatioFofFlow.setCoefficient1Constant(1.1552);
    energyInputRatioFofFlow.setCoefficient2x(-0.1808);
    energyInputRatioFofFlow.setCoefficient3xPOW2(0.0256);
    energyInputRatioFofFlow.setMinimumValueofx(0.5);
    energyInputRatioFofFlow.setMaximumValueofx(1.5);

    CurveQuadratic partLoadFraction = CurveQuadratic(model);
    partLoadFraction.setCoefficient1Constant(0.85);
    partLoadFraction.setCoefficient2x(0.15);
    partLoadFraction.setCoefficient3xPOW2(0.0);
    partLoadFraction.setMinimumValueofx(0.0);
    partLoadFraction.setMaximumValueofx(1.0);

    autosizeRatedTotalCoolingCapacity();  //autosize
    autosizeRatedSensibleHeatRatio();     //autosize
    autosizeRatedAirFlowRate();           //autosize
    setRatedCOP(3.0);
    setRatedEvaporatorFanPowerPerVolumeFlowRate2017(773.3);
    setRatedEvaporatorFanPowerPerVolumeFlowRate2023(934.4);
    setTotalCoolingCapacityFunctionOfTemperatureCurve(coolingCurveFofTemp);
    setTotalCoolingCapacityFunctionOfFlowFractionCurve(coolingCurveFofFlow);
    setEnergyInputRatioFunctionOfTemperatureCurve(energyInputRatioFofTemp);
    setEnergyInputRatioFunctionOfFlowFractionCurve(energyInputRatioFofFlow);
    setPartLoadFractionCorrelationCurve(partLoadFraction);
    autosizeEvaporativeCondenserAirFlowRate();                         //autosize
    autosizeEvaporativeCondenserPumpRatedPowerConsumption();           //autosize
    setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-25.0);  // Per E+ IDD default
    setNominalTimeForCondensateRemovalToBegin(0.0);
    setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(0.0);
    setMaximumCyclingRate(0.0);
    setLatentCapacityTimeConstant(0.0);
    setCondenserType("AirCooled");
    setEvaporativeCondenserEffectiveness(0.9);
    setCrankcaseHeaterCapacity(0.0);
    setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(10.0);
    setBasinHeaterCapacity(0.0);
    setBasinHeaterSetpointTemperature(2.0);

    auto schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);
  }

  // create a new CoilCoolingDXSingleSpeed object in the model's workspace
  CoilCoolingDXSingleSpeed::CoilCoolingDXSingleSpeed(const Model& model, Schedule& availabilitySchedule, const Curve& coolingCurveFofTemp,
                                                     const Curve& coolingCurveFofFlow, const Curve& energyInputRatioFofTemp,
                                                     const Curve& energyInputRatioFofFlow, const Curve& partLoadFraction)
    : StraightComponent(CoilCoolingDXSingleSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXSingleSpeed_Impl>());
    bool ok = setAvailabilitySchedule(availabilitySchedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to " << availabilitySchedule.briefDescription() << ".");
    }
    autosizeRatedTotalCoolingCapacity();  //autosize
    autosizeRatedSensibleHeatRatio();     //autosize
    autosizeRatedAirFlowRate();           //autosize
    setRatedCOP(3.0);
    setRatedEvaporatorFanPowerPerVolumeFlowRate2017(773.3);
    setRatedEvaporatorFanPowerPerVolumeFlowRate2023(934.4);
    setTotalCoolingCapacityFunctionOfTemperatureCurve(coolingCurveFofTemp);
    setTotalCoolingCapacityFunctionOfFlowFractionCurve(coolingCurveFofFlow);
    setEnergyInputRatioFunctionOfTemperatureCurve(energyInputRatioFofTemp);
    setEnergyInputRatioFunctionOfFlowFractionCurve(energyInputRatioFofFlow);
    setPartLoadFractionCorrelationCurve(partLoadFraction);
    autosizeEvaporativeCondenserAirFlowRate();                         //autosize
    autosizeEvaporativeCondenserPumpRatedPowerConsumption();           //autosize
    setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-25.0);  // Per E+ IDD default
    setNominalTimeForCondensateRemovalToBegin(0.0);
    setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(0.0);
    setMaximumCyclingRate(0.0);
    setLatentCapacityTimeConstant(0.0);
    setCondenserType("AirCooled");
    setEvaporativeCondenserEffectiveness(0.9);
    setCrankcaseHeaterCapacity(0.0);
    setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(10.0);
    setBasinHeaterCapacity(0.0);
    setBasinHeaterSetpointTemperature(2.0);
  }

  CoilCoolingDXSingleSpeed::CoilCoolingDXSingleSpeed(std::shared_ptr<detail::CoilCoolingDXSingleSpeed_Impl> p) : StraightComponent(std::move(p)) {}

  Schedule CoilCoolingDXSingleSpeed::availabilitySchedule() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->availabilitySchedule();
  }

  bool CoilCoolingDXSingleSpeed::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setAvailabilitySchedule(schedule);
  }

  double CoilCoolingDXSingleSpeed::ratedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedCOP();
  }

  bool CoilCoolingDXSingleSpeed::setRatedCOP(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedCOP(value);
  }

  bool CoilCoolingDXSingleSpeed::setRatedCOP(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedCOP(value);
  }

  double CoilCoolingDXSingleSpeed::ratedEvaporatorFanPowerPerVolumeFlowRate() const {
    LOG(Warn, "As of 3.5.0, ratedEvaporatorFanPowerPerVolumeFlowRate is deprecated. Use ratedSupplyFanPowerPerVolumeFlowRate2017 instead. It will be "
              "removed within three releases.");
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate2017();
  }

  double CoilCoolingDXSingleSpeed::ratedEvaporatorFanPowerPerVolumeFlowRate2017() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate2017();
  }

  double CoilCoolingDXSingleSpeed::ratedEvaporatorFanPowerPerVolumeFlowRate2023() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedEvaporatorFanPowerPerVolumeFlowRate2023();
  }

  bool CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate(boost::optional<double> value) {
    LOG(Warn, "As of 3.5.0, setRatedEvaporatorFanPowerPerVolumeFlowRate is deprecated. Use setRatedEvaporatorFanPowerPerVolumeFlowRate2017 instead. "
              "It will be removed within three releases.");
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate2017(value);
  }

  bool CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate(double value) {
    LOG(Warn, "As of 3.5.0, setRatedEvaporatorFanPowerPerVolumeFlowRate is deprecated. Use setRatedEvaporatorFanPowerPerVolumeFlowRate2017 instead. "
              "It will be removed within three releases.");
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate2017(value);
  }

  bool CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate2017(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate2017(value);
  }

  bool CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate2017(value);
  }

  bool CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate2023(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate2023(value);
  }

  bool CoilCoolingDXSingleSpeed::setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedEvaporatorFanPowerPerVolumeFlowRate2023(value);
  }

  Curve CoilCoolingDXSingleSpeed::totalCoolingCapacityFunctionOfTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->totalCoolingCapacityFunctionOfTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeed::setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setTotalCoolingCapacityFunctionOfTemperatureCurve(curve);
  }

  Curve CoilCoolingDXSingleSpeed::totalCoolingCapacityFunctionOfFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->totalCoolingCapacityFunctionOfFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeed::setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setTotalCoolingCapacityFunctionOfFlowFractionCurve(curve);
  }

  Curve CoilCoolingDXSingleSpeed::energyInputRatioFunctionOfTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->energyInputRatioFunctionOfTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeed::setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEnergyInputRatioFunctionOfTemperatureCurve(curve);
  }

  Curve CoilCoolingDXSingleSpeed::energyInputRatioFunctionOfFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->energyInputRatioFunctionOfFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeed::setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEnergyInputRatioFunctionOfFlowFractionCurve(curve);
  }

  Curve CoilCoolingDXSingleSpeed::partLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->partLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeed::setPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setPartLoadFractionCorrelationCurve(curve);
  }

  double CoilCoolingDXSingleSpeed::nominalTimeForCondensateRemovalToBegin() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->nominalTimeForCondensateRemovalToBegin();
  }

  bool CoilCoolingDXSingleSpeed::setNominalTimeForCondensateRemovalToBegin(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setNominalTimeForCondensateRemovalToBegin(value);
  }

  bool CoilCoolingDXSingleSpeed::setNominalTimeForCondensateRemovalToBegin(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setNominalTimeForCondensateRemovalToBegin(value);
  }

  double CoilCoolingDXSingleSpeed::ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity();
  }

  bool CoilCoolingDXSingleSpeed::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(value);
  }
  bool CoilCoolingDXSingleSpeed::setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(value);
  }

  double CoilCoolingDXSingleSpeed::maximumCyclingRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->maximumCyclingRate();
  }

  bool CoilCoolingDXSingleSpeed::setMaximumCyclingRate(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumCyclingRate(value);
  }

  bool CoilCoolingDXSingleSpeed::setMaximumCyclingRate(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumCyclingRate(value);
  }

  double CoilCoolingDXSingleSpeed::latentCapacityTimeConstant() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->latentCapacityTimeConstant();
  }

  bool CoilCoolingDXSingleSpeed::setLatentCapacityTimeConstant(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setLatentCapacityTimeConstant(value);
  }

  bool CoilCoolingDXSingleSpeed::setLatentCapacityTimeConstant(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setLatentCapacityTimeConstant(value);
  }

  boost::optional<std::string> CoilCoolingDXSingleSpeed::condenserAirInletNodeName() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->condenserAirInletNodeName();
  }

  bool CoilCoolingDXSingleSpeed::setCondenserAirInletNodeName(const boost::optional<std::string>& value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCondenserAirInletNodeName(value);
  }

  std::string CoilCoolingDXSingleSpeed::condenserType() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->condenserType();
  }

  bool CoilCoolingDXSingleSpeed::setCondenserType(const std::string& value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCondenserType(value);
  }

  double CoilCoolingDXSingleSpeed::evaporativeCondenserEffectiveness() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserEffectiveness();
  }

  bool CoilCoolingDXSingleSpeed::setEvaporativeCondenserEffectiveness(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserEffectiveness(value);
  }

  bool CoilCoolingDXSingleSpeed::setEvaporativeCondenserEffectiveness(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserEffectiveness(value);
  }

  OptionalDouble CoilCoolingDXSingleSpeed::evaporativeCondenserAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeed::isEvaporativeCondenserAirFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
  }

  bool CoilCoolingDXSingleSpeed::setEvaporativeCondenserAirFlowRate(OptionalDouble value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserAirFlowRate(value);
  }

  bool CoilCoolingDXSingleSpeed::setEvaporativeCondenserAirFlowRate(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserAirFlowRate(value);
  }

  void CoilCoolingDXSingleSpeed::autosizeEvaporativeCondenserAirFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeed::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
  }

  OptionalDouble CoilCoolingDXSingleSpeed::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
  }

  bool CoilCoolingDXSingleSpeed::setEvaporativeCondenserPumpRatedPowerConsumption(OptionalDouble value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(value);
  }

  bool CoilCoolingDXSingleSpeed::setEvaporativeCondenserPumpRatedPowerConsumption(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(value);
  }

  void CoilCoolingDXSingleSpeed::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  }

  double CoilCoolingDXSingleSpeed::crankcaseHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->crankcaseHeaterCapacity();
  }

  bool CoilCoolingDXSingleSpeed::setCrankcaseHeaterCapacity(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCrankcaseHeaterCapacity(value);
  }

  bool CoilCoolingDXSingleSpeed::setCrankcaseHeaterCapacity(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setCrankcaseHeaterCapacity(value);
  }

  double CoilCoolingDXSingleSpeed::maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation();
  }

  bool CoilCoolingDXSingleSpeed::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(value);
  }

  bool CoilCoolingDXSingleSpeed::setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(value);
  }

  //getSupplyWaterStorageTankName
  //setSupplyWaterStorageTankName

  //getCondensateCollectionWaterStorageTankName
  //setCondensateCollectionWaterStorageTankName

  double CoilCoolingDXSingleSpeed::basinHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterCapacity();
  }

  bool CoilCoolingDXSingleSpeed::setBasinHeaterCapacity(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterCapacity(value);
  }

  bool CoilCoolingDXSingleSpeed::setBasinHeaterCapacity(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterCapacity(value);
  }

  double CoilCoolingDXSingleSpeed::basinHeaterSetpointTemperature() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterSetpointTemperature();
  }

  bool CoilCoolingDXSingleSpeed::setBasinHeaterSetpointTemperature(boost::optional<double> value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterSetpointTemperature(value);
  }

  bool CoilCoolingDXSingleSpeed::setBasinHeaterSetpointTemperature(double value) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterSetpointTemperature(value);
  }

  OptionalSchedule CoilCoolingDXSingleSpeed::basinHeaterOperatingSchedule() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->basinHeaterOperatingSchedule();
  }

  bool CoilCoolingDXSingleSpeed::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setBasinHeaterOperatingSchedule(schedule);
  }

  void CoilCoolingDXSingleSpeed::resetBasinHeaterOperatingSchedule() {
    getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->resetBasinHeaterOperatingSchedule();
  }

  IddObjectType CoilCoolingDXSingleSpeed::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Coil_Cooling_DX_SingleSpeed);
    return result;
  }

  std::vector<std::string> CoilCoolingDXSingleSpeed::condenserTypeValues() const {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Cooling_DX_SingleSpeedFields::CondenserType);
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::ratedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedTotalCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeed::isRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isRatedTotalCoolingCapacityAutosized();
  }

  bool CoilCoolingDXSingleSpeed::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedTotalCoolingCapacity(ratedTotalCoolingCapacity);
  }

  void CoilCoolingDXSingleSpeed::autosizeRatedTotalCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeRatedTotalCoolingCapacity();
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::ratedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedSensibleHeatRatio();
  }

  bool CoilCoolingDXSingleSpeed::isRatedSensibleHeatRatioAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isRatedSensibleHeatRatioAutosized();
  }

  bool CoilCoolingDXSingleSpeed::setRatedSensibleHeatRatio(double ratedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedSensibleHeatRatio(ratedSensibleHeatRatio);
  }

  void CoilCoolingDXSingleSpeed::autosizeRatedSensibleHeatRatio() {
    getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeRatedSensibleHeatRatio();
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::ratedAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->ratedAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeed::isRatedAirFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->isRatedAirFlowRateAutosized();
  }

  bool CoilCoolingDXSingleSpeed::setRatedAirFlowRate(double ratedAirFlowRate) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
  }

  void CoilCoolingDXSingleSpeed::autosizeRatedAirFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizeRatedAirFlowRate();
  }

  double CoilCoolingDXSingleSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
  }

  bool CoilCoolingDXSingleSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
    double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      minimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  AirflowNetworkEquivalentDuct CoilCoolingDXSingleSpeed::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> CoilCoolingDXSingleSpeed::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->airflowNetworkEquivalentDuct();
  }

  // Autosizing methods

  boost::optional<double> CoilCoolingDXSingleSpeed::autosizedRatedAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedRatedAirFlowRate();
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::autosizedRatedTotalCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedRatedTotalCoolingCapacity();
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::autosizedRatedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedRatedSensibleHeatRatio();
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::autosizedEvaporativeCondenserAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedEvaporativeCondenserAirFlowRate();
  }

  boost::optional<double> CoilCoolingDXSingleSpeed::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedEvaporativeCondenserPumpRatedPowerConsumption();
  }

}  // namespace model
}  // namespace openstudio
