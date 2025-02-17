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

#include "ProgramControl.hpp"
#include "ProgramControl_Impl.hpp"

#include <utilities/idd/OS_ProgramControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ProgramControl_Impl::ProgramControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ProgramControl::iddObjectType());
    }

    ProgramControl_Impl::ProgramControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ProgramControl::iddObjectType());
    }

    ProgramControl_Impl::ProgramControl_Impl(const ProgramControl_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ProgramControl_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ProgramControl_Impl::iddObjectType() const {
      return ProgramControl::iddObjectType();
    }

    boost::optional<int> ProgramControl_Impl::numberofThreadsAllowed() const {
      return getInt(OS_ProgramControlFields::NumberofThreadsAllowed, true);
    }

    bool ProgramControl_Impl::setNumberofThreadsAllowed(boost::optional<int> numberofThreadsAllowed) {
      bool result(false);
      if (numberofThreadsAllowed) {
        result = setInt(OS_ProgramControlFields::NumberofThreadsAllowed, numberofThreadsAllowed.get());
      } else {
        resetNumberofThreadsAllowed();
        result = true;
      }
      return result;
    }

    void ProgramControl_Impl::resetNumberofThreadsAllowed() {
      bool result = setString(OS_ProgramControlFields::NumberofThreadsAllowed, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ProgramControl::ProgramControl(const Model& model) : ModelObject(ProgramControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ProgramControl_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
  }

  IddObjectType ProgramControl::iddObjectType() {
    return {IddObjectType::OS_ProgramControl};
  }

  boost::optional<int> ProgramControl::numberofThreadsAllowed() const {
    return getImpl<detail::ProgramControl_Impl>()->numberofThreadsAllowed();
  }

  bool ProgramControl::setNumberofThreadsAllowed(int numberofThreadsAllowed) {
    return getImpl<detail::ProgramControl_Impl>()->setNumberofThreadsAllowed(numberofThreadsAllowed);
  }

  void ProgramControl::resetNumberofThreadsAllowed() {
    getImpl<detail::ProgramControl_Impl>()->resetNumberofThreadsAllowed();
  }

  /// @cond
  ProgramControl::ProgramControl(std::shared_ptr<detail::ProgramControl_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
