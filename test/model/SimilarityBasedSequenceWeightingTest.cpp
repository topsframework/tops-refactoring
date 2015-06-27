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

// Standard headers
#include <math.h>
#include <vector>

// External headers
#include "gmock/gmock.h"

// ToPS headers
#include "model/SimilarityBasedSequenceWeighting.hpp"
#include "model/Sequence.hpp"
#include "model/Random.hpp"

using ::testing::Eq;
using ::testing::DoubleEq;
using ::testing::DoubleNear;
using ::testing::ContainerEq;

using tops::model::SimilarityBasedSequenceWeighting;
using tops::model::SimilarityBasedSequenceWeightingPtr;
using tops::model::INVALID_SYMBOL;
using tops::model::Sequence;

class ASBSW : public testing::Test {
 protected:
  SimilarityBasedSequenceWeightingPtr sbsw;

  virtual void SetUp() {
    std::map<Sequence, double> counter;
    counter[{1, 1}] = 2;
    counter[{0, 0}] = 4;
    sbsw = SimilarityBasedSequenceWeighting::make(counter, 6, -1, -1, {1, 0});
  }
};

TEST_F(ASBSW, ShouldEvaluateASequence) {
  ASSERT_THAT(sbsw->evaluator({0})->probabilityOf(0, 1),
              DoubleEq(-HUGE));
  ASSERT_THAT(sbsw->evaluator({1})->probabilityOf(0, 1),
              DoubleEq(-HUGE));
  ASSERT_THAT(sbsw->evaluator({0, 1})->probabilityOf(0, 2),
              DoubleNear(-6.90776, 1e-4));
  ASSERT_THAT(sbsw->evaluator({0, 0})->probabilityOf(0, 2),
              DoubleNear(-0.405465, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0})->probabilityOf(0, 2),
              DoubleNear(-6.90776, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 1})->probabilityOf(0, 2),
              DoubleNear(-1.09861, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0, 1})->probabilityOf(0, 3),
              DoubleNear(-6.90776, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0, 1, 0})->probabilityOf(0, 4),
              DoubleNear(-6.90776, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})->probabilityOf(0, 13),
              DoubleNear(-6.90776, 1e-4));
}

TEST_F(ASBSW, ShouldEvaluateASequenceWithPrefixSumArray) {
  ASSERT_THAT(sbsw->evaluator({0}, true)->probabilityOf(0, 1),
              DoubleNear(sbsw->evaluator({0})->probabilityOf(0, 1), 1e-4));

  ASSERT_THAT(sbsw->evaluator({0, 1}, true)->probabilityOf(0, 2),
              DoubleNear(sbsw->evaluator({0, 1})->probabilityOf(0, 2), 1e-4));

  ASSERT_THAT(sbsw->evaluator({1, 0, 1}, true)->probabilityOf(0, 3),
              DoubleNear(sbsw->evaluator({1, 0, 1})->probabilityOf(0, 3), 1e-4));

  ASSERT_THAT(sbsw->evaluator({1, 0, 1, 0}, true)->probabilityOf(0, 4),
              DoubleNear(sbsw->evaluator({1, 0, 1, 0})->probabilityOf(0, 4), 1e-4));

  ASSERT_THAT(sbsw->evaluator({1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, true)->probabilityOf(0, 13),
              DoubleNear(sbsw->evaluator({1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})->probabilityOf(0, 13), 1e-4));
}

TEST_F(ASBSW, ShouldChooseSequenceWithSeed42) {
  // TODO(igorbonadio): check bigger sequence
  tops::model::resetRandom();
  ASSERT_THAT(sbsw->standardGenerator()->drawSequence(5), ContainerEq(Sequence(5, INVALID_SYMBOL)));
}

TEST(SBSW, ShouldBeTrained) {
  std::vector<Sequence> training_set = {
    {1, 1},
    {0, 1},
    {1, 1},
    {0, 0},
    {1, 1},
  };
  auto sbsw = SimilarityBasedSequenceWeighting::train(training_set, 2, -1, -1, {});
  ASSERT_THAT(sbsw->evaluator({0})->probabilityOf(0, 1),
              DoubleEq(-HUGE));
  ASSERT_THAT(sbsw->evaluator({1})->probabilityOf(0, 1),
              DoubleEq(-HUGE));
  ASSERT_THAT(sbsw->evaluator({0, 1})->probabilityOf(0, 2),
              DoubleNear(-1.60684, 1e-4));
  ASSERT_THAT(sbsw->evaluator({0, 0})->probabilityOf(0, 2),
              DoubleNear(-1.60984, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0})->probabilityOf(0, 2),
              DoubleNear(-7.1323, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 1})->probabilityOf(0, 2),
              DoubleNear(-0.511891, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0, 1})->probabilityOf(0, 3),
              DoubleNear(-7.1323, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0, 1, 0})->probabilityOf(0, 4),
              DoubleNear(-7.1323, 1e-4));
  ASSERT_THAT(sbsw->evaluator({1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})->probabilityOf(0, 13),
              DoubleNear(-7.1323, 1e-4));
}
