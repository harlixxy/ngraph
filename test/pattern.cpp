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

#include <cstdio>
#include <memory>
#include "gtest/gtest.h"

#include "ngraph/ngraph.hpp"

#include "ngraph/pattern/matcher.hpp"
#include "ngraph/pattern/op/any.hpp"

class TestMatcher : public ngraph::pattern::Matcher
{
public:
    bool compare_nodes(const std::shared_ptr<ngraph::Node>& pattern_node,
                       const std::shared_ptr<ngraph::Node>& graph_node);

    virtual void on_match_class(const std::shared_ptr<ngraph::Node>& pattern_node,
                                const std::shared_ptr<ngraph::Node>& graph_node,
                                bool is_match);

    std::shared_ptr<ngraph::Node> get_pattern_node() const { return m_pattern_node; }
    std::shared_ptr<ngraph::Node> get_graph_node() const { return m_graph_node; }
    bool get_is_valid() const { return m_is_valid; }
protected:
    bool m_is_valid;
    bool m_is_match;
    std::shared_ptr<ngraph::Node> m_pattern_node;
    std::shared_ptr<ngraph::Node> m_graph_node;
};

void TestMatcher::on_match_class(const std::shared_ptr<ngraph::Node>& pattern_node,
                                 const std::shared_ptr<ngraph::Node>& graph_node,
                                 bool is_match)
{
    m_pattern_node = pattern_node;
    m_graph_node = graph_node;
    m_is_match = is_match;
    m_is_valid = true;
}

bool TestMatcher::compare_nodes(const std::shared_ptr<ngraph::Node>& pattern_node,
                                const std::shared_ptr<ngraph::Node>& graph_node)
{
    m_pattern_node = nullptr;
    m_graph_node = nullptr;
    m_is_match = false;
    m_is_valid = false;
    pattern_node->match_class(*this, graph_node);
    return m_is_match;
}

using namespace ngraph;
using namespace std;

TEST(pattern, op_op)
{
    auto shape = Shape{1};

    auto a = make_shared<op::Parameter>(element::Int32::element_type(), shape);
    pattern::Matcher n;

    ASSERT_TRUE(n.match(a, a));
    n.reset();
    auto pattern = std::make_shared<pattern::op::Pattern>();
    ASSERT_TRUE(n.match(pattern, a));
    ASSERT_EQ(pattern->get_binded_node(), a);
    pattern->reset();
    ASSERT_FALSE(pattern->is_binded());

    auto b = make_shared<op::Parameter>(element::Int32::element_type(), shape);
    auto sum = a + b;

    auto any = std::make_shared<pattern::op::Any>();
    auto any_sum = any + b;

    n.reset();
    ASSERT_TRUE(n.match(pattern, a));
    n.reset();
    ASSERT_TRUE(n.match(any_sum, sum));
    n.reset();
    auto pattern_sum = pattern + b;
    ASSERT_TRUE(n.match(pattern_sum, sum));
    ASSERT_EQ(pattern->get_binded_node(), a);
    n.reset();
    pattern->reset();
    auto pattern_sum_perm = b + pattern;
    ASSERT_TRUE(n.match(pattern_sum_perm, sum));
    ASSERT_EQ(pattern->get_binded_node(), b);
    //auto c = make_shared<op::Parameter>(element::Int32::element_type(), shape);
    //
    //{AT}
    //ASSERT_TRUE(n.match(ANY, A));

    //auto PATTERN3 = std::make_shared<pattern::op::Pattern>();
    //auto PATTERN_SUM = A + PATTERN3;
    //ASSERT_TRUE(n.match(PATTERN_SUM, SUM);

    //auto PATTERN_SUM_PERM = B + PATTERN;
    //ASSERT_TRUE(PATTERN_SUM_PERM, SUM);

    //auto PATTERN2 = std::make_shared<pattern::op::Pattern>();
    //auto ANYmulPATTERN2 = ANY*PATTERN2;
    //auto ANYmulPATTERN2_2 = ANY*PATTERN2;
    //A*B + C*B

    //auto AmulB = A * B;
    //auto BmulC = B * C;
    //auto SumOfMul = AmulB + BmulC;

    //TestMatcher test_matcher;
    /*ASSERT_TRUE(test_matcher.compare_nodes(A, B));
    ASSERT_FALSE(test_matcher.compare_nodes(A, SUM));
    ASSERT_TRUE(test_matcher.compare_nodes(ANY, A));
    ASSERT_TRUE(test_matcher.compare_nodes(ANY, B));
    ASSERT_TRUE(test_matcher.compare_nodes(ANY, SUM));*/
}

/*
TEST(pattern, op_op)
{
    auto shape = Shape{2, 2};
    auto A = make_shared<op::Parameter>(element::Float32::element_type(), shape);
    auto B = make_shared<op::Parameter>(element::Float32::element_type(), shape);
    auto SUM = A + B;
    auto ANY = std::make_shared<pattern::op::Any>();

    TestMatcher test_matcher;
    ASSERT_TRUE(test_matcher.compare_nodes(A, B));
    ASSERT_FALSE(test_matcher.compare_nodes(A, SUM));
    ASSERT_TRUE(test_matcher.compare_nodes(ANY, A));
    ASSERT_TRUE(test_matcher.compare_nodes(ANY, B));
    ASSERT_TRUE(test_matcher.compare_nodes(ANY, SUM));
}
*/