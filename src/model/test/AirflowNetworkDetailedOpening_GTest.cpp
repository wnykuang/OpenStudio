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

#include "../AirflowNetworkDetailedOpening.hpp"
#include "../AirflowNetworkDetailedOpening_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_DetailedOpening) {
  Model model;

  std::vector<DetailedOpeningFactorData> data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0),
                                                 DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};

  AirflowNetworkDetailedOpening detailed0(model, 1.0, data);
  AirflowNetworkDetailedOpening detailed1(model, 1.0, 0.5, "HorizontallyPivoted", 0.0, data);

  EXPECT_EQ(1, detailed0.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.65, detailed0.airMassFlowExponentWhenOpeningisClosed());
  EXPECT_EQ("NonPivoted", detailed0.typeofRectangularLargeVerticalOpening());
  std::vector<DetailedOpeningFactorData> retData = detailed0.openingFactors();
  ASSERT_EQ(2u, retData.size());
  EXPECT_EQ(0, retData[0].openingFactor());
  EXPECT_EQ(1, retData[1].openingFactor());

  EXPECT_EQ(1, detailed1.airMassFlowCoefficientWhenOpeningisClosed());
  EXPECT_EQ(0.5, detailed1.airMassFlowExponentWhenOpeningisClosed());
  EXPECT_EQ("HorizontallyPivoted", detailed1.typeofRectangularLargeVerticalOpening());
  retData = detailed1.openingFactors();
  ASSERT_EQ(2u, retData.size());
  EXPECT_EQ(0, retData[0].openingFactor());
  EXPECT_EQ(1, retData[1].openingFactor());
}

TEST_F(ModelFixture, AirflowNetwork_DetailedOpening_InvalidOpeningFactors) {
  Model model;

  std::vector<DetailedOpeningFactorData> data;

  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};

  AirflowNetworkDetailedOpening detailed0(model, 1.0, data);

  EXPECT_TRUE(detailed0.setOpeningFactors(data));

  // Number of Sets of Opening Factor Data
  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0),
          DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0),
          DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  // Opening Factor
  data = {DetailedOpeningFactorData(0.5, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(0.5, 0.5, 1.0, 1.0, 0.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));

  // Height Factor and Start Height Factor
  data = {DetailedOpeningFactorData(0.0, 0.01, 0.0, 0.0, 0.0), DetailedOpeningFactorData(1.0, 0.5, 1.0, 1.0, 1.0)};
  EXPECT_FALSE(detailed0.setOpeningFactors(data));
}
