/***********************************************************************/
/*  Copyright 2015 ToPS                                                */
/*                                                                     */
/*  This program is free software; you can redistribute it and/or      */
/*  modify it under the terms of the GNU  General Public License as    */
/*  published by the Free Software Foundation; either version 3 of     */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This program is distributed in the hope that it will be useful,    */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      */
/*  GNU General Public License for more details.                       */
/*                                                                     */
/*  You should have received a copy of the GNU General Public License  */
/*  along with this program; if not, write to the Free Software        */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,         */
/*  MA 02110-1301, USA.                                                */
/***********************************************************************/

#include <cmath>
#include <vector>

#include "gmock/gmock.h"

#include "model/DiscreteIIDModel.hpp"
#include "model/Sequence.hpp"

#include "helper/DiscreteIIDModel.hpp"
#include "helper/Sequence.hpp"

using ::testing::Eq;
using ::testing::DoubleEq;

using tops::model::DiscreteIIDModel;
using tops::model::DiscreteIIDModelPtr;
using tops::model::Sequence;

using tops::helper::createLoadedCoinIIDModel;
using tops::helper::generateRandomSequence;

class ADiscreteIIDModel : public testing::Test {
 protected:
  DiscreteIIDModelPtr iid = createLoadedCoinIIDModel();
};

TEST_F(ADiscreteIIDModel, ShouldHaveAnAlphabetSize) {
  ASSERT_THAT(iid->alphabetSize(), Eq(2));
}

TEST_F(ADiscreteIIDModel, ShouldEvaluateASingleSymbol) {
  ASSERT_THAT(iid->probabilityOf(0), DoubleEq(log(0.2)));
  ASSERT_THAT(iid->probabilityOf(1), DoubleEq(log(0.8)));
}

TEST_F(ADiscreteIIDModel, ShouldHaveEvaluateASequence) {
  std::vector<Sequence> test_data = {
    {0, 0, 1, 1},
    {0, 1, 1, 1},
    {1, 1, 1, 1}
  };
  for (auto data : test_data) {
    double result = 0.0;
    for (auto symbol : data) {
      result += iid->probabilityOf(symbol);
    }
    ASSERT_THAT(iid->evaluateSequence(data, 0, 4), DoubleEq(result));
  }
}

TEST_F(ADiscreteIIDModel, ShouldEvaluateASequencePosition) {
  ASSERT_THAT(iid->evaluatePosition({0, 1, 0}, 0), DoubleEq(log(0.2)));
  ASSERT_THAT(iid->evaluatePosition({0, 1, 0}, 1), DoubleEq(log(0.8)));
  ASSERT_THAT(iid->evaluatePosition({0, 1, 0}, 2), DoubleEq(log(0.2)));
}

TEST_F(ADiscreteIIDModel, ShouldEvaluateASequenceWithPrefixSumArray) {
  for (int i = 1; i < 1000; i++) {
    auto data = generateRandomSequence(i, 2);
    iid->initializePrefixSumArray(data);
    ASSERT_THAT(iid->evaluateWithPrefixSumArray(0, data.size()), DoubleEq(iid->evaluateSequence(data, 0, data.size())));
  }
}
