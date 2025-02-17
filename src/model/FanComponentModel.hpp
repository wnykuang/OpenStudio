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

#ifndef MODEL_FANCOMPONENTMODEL_HPP
#define MODEL_FANCOMPONENTMODEL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Schedule;
  class AirflowNetworkFan;

  namespace detail {

    class FanComponentModel_Impl;

  }  // namespace detail

  /** FanComponentModel is a StraightComponent that wraps the OpenStudio IDD object 'OS:Fan:ComponentModel'. */
  class MODEL_API FanComponentModel : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Constructor that will instantiate all required curves. It will **not** instantiate the optional curve objects.
    // You can then call the helper method `bool assignDefaultOptionalCurves()` if that is what you want
    explicit FanComponentModel(const Model& model);

    // Explicit constructor that takes in all required curves. It will **not** instantiate the optional curve objects.
    // You can then call the helper method `bool assignDefaultOptionalCurves()` if that is what you want
    explicit FanComponentModel(const Model& model, const Curve& fanPressureRiseCurve, const Curve& ductStaticPressureResetCurve,
                               const Curve& normalizedFanStaticEfficiencyCurveNonStallRegion,
                               const Curve& normalizedFanStaticEfficiencyCurveStallRegion,
                               const Curve& normalizedDimensionlessAirflowCurveNonStallRegion,
                               const Curve& normalizedDimensionlessAirflowCurveStallRegion);

    virtual ~FanComponentModel() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanComponentModel(const FanComponentModel& other) = default;
    FanComponentModel(FanComponentModel&& other) = default;
    FanComponentModel& operator=(const FanComponentModel&) = default;
    FanComponentModel& operator=(FanComponentModel&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> vFDEfficiencyTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumFlowRate() const;

    bool isMaximumFlowRateAutosized() const;

    boost::optional<double> minimumFlowRate() const;

    bool isMinimumFlowRateAutosized() const;

    double fanSizingFactor() const;

    double fanWheelDiameter() const;

    double fanOutletArea() const;

    double maximumFanStaticEfficiency() const;

    double eulerNumberatMaximumFanStaticEfficiency() const;

    double maximumDimensionlessFanAirflow() const;

    boost::optional<double> motorFanPulleyRatio() const;

    bool isMotorFanPulleyRatioAutosized() const;

    boost::optional<double> beltMaximumTorque() const;

    bool isBeltMaximumTorqueAutosized() const;

    double beltSizingFactor() const;

    double beltFractionalTorqueTransition() const;

    double motorMaximumSpeed() const;

    boost::optional<double> maximumMotorOutputPower() const;

    bool isMaximumMotorOutputPowerAutosized() const;

    double motorSizingFactor() const;

    double motorInAirstreamFraction() const;

    std::string vFDEfficiencyType() const;

    boost::optional<double> maximumVFDOutputPower() const;

    bool isMaximumVFDOutputPowerAutosized() const;

    double vFDSizingFactor() const;

    Curve fanPressureRiseCurve() const;

    Curve ductStaticPressureResetCurve() const;

    Curve normalizedFanStaticEfficiencyCurveNonStallRegion() const;

    Curve normalizedFanStaticEfficiencyCurveStallRegion() const;

    Curve normalizedDimensionlessAirflowCurveNonStallRegion() const;

    Curve normalizedDimensionlessAirflowCurveStallRegion() const;

    boost::optional<Curve> maximumBeltEfficiencyCurve() const;

    boost::optional<Curve> normalizedBeltEfficiencyCurveRegion1() const;

    boost::optional<Curve> normalizedBeltEfficiencyCurveRegion2() const;

    boost::optional<Curve> normalizedBeltEfficiencyCurveRegion3() const;

    boost::optional<Curve> maximumMotorEfficiencyCurve() const;

    boost::optional<Curve> normalizedMotorEfficiencyCurve() const;

    boost::optional<Curve> vFDEfficiencyCurve() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumFlowRate(double maximumFlowRate);

    void autosizeMaximumFlowRate();

    bool setMinimumFlowRate(double minimumFlowRate);

    void autosizeMinimumFlowRate();

    bool setFanSizingFactor(double fanSizingFactor);

    bool setFanWheelDiameter(double fanWheelDiameter);

    bool setFanOutletArea(double fanOutletArea);

    bool setMaximumFanStaticEfficiency(double maximumFanStaticEfficiency);

    bool setEulerNumberatMaximumFanStaticEfficiency(double eulerNumberatMaximumFanStaticEfficiency);

    bool setMaximumDimensionlessFanAirflow(double maximumDimensionlessFanAirflow);

    bool setMotorFanPulleyRatio(double motorFanPulleyRatio);

    void autosizeMotorFanPulleyRatio();

    bool setBeltMaximumTorque(double beltMaximumTorque);

    void autosizeBeltMaximumTorque();

    bool setBeltSizingFactor(double beltSizingFactor);

    bool setBeltFractionalTorqueTransition(double beltFractionalTorqueTransition);

    bool setMotorMaximumSpeed(double motorMaximumSpeed);

    bool setMaximumMotorOutputPower(double maximumMotorOutputPower);

    void autosizeMaximumMotorOutputPower();

    bool setMotorSizingFactor(double motorSizingFactor);

    bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

    bool setVFDEfficiencyType(const std::string& vFDEfficiencyType);

    bool setMaximumVFDOutputPower(double maximumVFDOutputPower);

    void autosizeMaximumVFDOutputPower();

    bool setVFDSizingFactor(double vFDSizingFactor);

    bool setFanPressureRiseCurve(const Curve& bivariateFunctions);

    bool setDuctStaticPressureResetCurve(const Curve& univariateFunctions);

    bool setNormalizedFanStaticEfficiencyCurveNonStallRegion(const Curve& univariateFunctions);

    bool setNormalizedFanStaticEfficiencyCurveStallRegion(const Curve& univariateFunctions);

    bool setNormalizedDimensionlessAirflowCurveNonStallRegion(const Curve& univariateFunctions);

    bool setNormalizedDimensionlessAirflowCurveStallRegion(const Curve& univariateFunctions);

    bool setMaximumBeltEfficiencyCurve(const Curve& univariateFunctions);

    void resetMaximumBeltEfficiencyCurve();

    bool setNormalizedBeltEfficiencyCurveRegion1(const Curve& univariateFunctions);

    void resetNormalizedBeltEfficiencyCurveRegion1();

    bool setNormalizedBeltEfficiencyCurveRegion2(const Curve& univariateFunctions);

    void resetNormalizedBeltEfficiencyCurveRegion2();

    bool setNormalizedBeltEfficiencyCurveRegion3(const Curve& univariateFunctions);

    void resetNormalizedBeltEfficiencyCurveRegion3();

    bool setMaximumMotorEfficiencyCurve(const Curve& univariateFunctions);

    void resetMaximumMotorEfficiencyCurve();

    bool setNormalizedMotorEfficiencyCurve(const Curve& univariateFunctions);

    void resetNormalizedMotorEfficiencyCurve();

    bool setVFDEfficiencyCurve(const Curve& univariateFunctions);

    void resetVFDEfficiencyCurve();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    // Helper that creates defaulted Optional Curves
    bool assignDefaultOptionalCurves();

    boost::optional<double> autosizedMaximumFlowRate();
    boost::optional<double> autosizedMinimumFlowRate();
    boost::optional<double> autosizedMotorFanPulleyRatio();
    boost::optional<double> autosizedBeltMaximumTorque();
    boost::optional<double> autosizedMaximumMotorOutputPower();
    boost::optional<double> autosizedMaximumVFDOutputPower();

    //@}
   protected:
    /// @cond
    using ImplType = detail::FanComponentModel_Impl;

    explicit FanComponentModel(std::shared_ptr<detail::FanComponentModel_Impl> impl);

    friend class detail::FanComponentModel_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanComponentModel");
  };

  /** \relates FanComponentModel*/
  using OptionalFanComponentModel = boost::optional<FanComponentModel>;

  /** \relates FanComponentModel*/
  using FanComponentModelVector = std::vector<FanComponentModel>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANCOMPONENTMODEL_HPP
