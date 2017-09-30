// ----------------------------------------------------------------------------
// Copyright 2017 Nervana Systems Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// ----------------------------------------------------------------------------

#include "ngraph/pass/pass.hpp"
#include "ngraph/pass/manager.hpp"

ngraph::pass::ManagerState& ngraph::pass::Base::get_state()
{
    return *m_state;
}

void ngraph::pass::Base::set_state(ManagerState& state)
{
    m_state = &state;
}