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
#include <cmath>
#include <vector>

// External headers
#include "gmock/gmock.h"

// ToPS headers
#include "model/GeneralizedHiddenMarkovModel.hpp"
#include "model/Sequence.hpp"
#include "model/Matrix.hpp"
#include "model/Util.hpp"

#include "helper/DiscreteIIDModel.hpp"
#include "helper/VariableLengthMarkovChain.hpp"
#include "helper/Sequence.hpp"

using ::testing::Eq;
using ::testing::DoubleEq;
using ::testing::DoubleNear;
using ::testing::ContainerEq;

using tops::model::DiscreteIIDModel;
using tops::model::DiscreteIIDModelPtr;
using tops::model::GeneralizedHiddenMarkovModel;
using tops::model::GeneralizedHiddenMarkovModelPtr;
using tops::model::GeneralizedHiddenMarkovModelState;
using tops::model::GeneralizedHiddenMarkovModelStatePtr;
using tops::model::GeneralizedHiddenMarkovModelSignalState;
using tops::model::GeneralizedHiddenMarkovModelSignalStatePtr;
using tops::model::GeneralizedHiddenMarkovModelExplicitDurationState;
using tops::model::GeneralizedHiddenMarkovModelExplicitDurationStatePtr;
using tops::model::Sequence;
using tops::model::Matrix;
using tops::model::log_sum;
using tops::model::Labeling;

using tops::helper::createMachlerVLMC;
using tops::helper::createVLMCMC;
using tops::helper::createFairCoinIIDModel;
using tops::helper::generateAllCombinationsOfSymbols;

class AGHMM : public testing::Test {
 protected:
  GeneralizedHiddenMarkovModelStatePtr geometric_state
    = GeneralizedHiddenMarkovModelState::make(
      0, createMachlerVLMC(), DiscreteIIDModel::make(
        {log(0.3), log(0.3), log(0.4)}));
  GeneralizedHiddenMarkovModelStatePtr signal_state
    = GeneralizedHiddenMarkovModelSignalState::make(
    1, createVLMCMC(), DiscreteIIDModel::make(
        {log(0.1), -HUGE, log(0.9)}), 3);
  GeneralizedHiddenMarkovModelStatePtr explicit_duration_state
    = GeneralizedHiddenMarkovModelExplicitDurationState::make(
      2, createFairCoinIIDModel(),
      DiscreteIIDModel::make({0, -HUGE, -HUGE}),
      DiscreteIIDModel::make(
        {log(0.1), log(0.1), log(0.1), log(0.1), log(0.1), log(0.1), log(0.3), log(0.1)}));

  GeneralizedHiddenMarkovModelPtr ghmm = GeneralizedHiddenMarkovModel::make(
    {geometric_state, signal_state, explicit_duration_state},
    DiscreteIIDModel::make({0, -HUGE, -HUGE}),
    3, 2);

  virtual void SetUp() {
    geometric_state->addSuccessor(0);
    geometric_state->addSuccessor(1);
    geometric_state->addSuccessor(2);
    geometric_state->addPredecessor(0);
    geometric_state->addPredecessor(1);
    geometric_state->addPredecessor(2);

    signal_state->addSuccessor(0);
    signal_state->addSuccessor(2);
    signal_state->addPredecessor(0);

    explicit_duration_state->addSuccessor(0);
    explicit_duration_state->addPredecessor(0);
    explicit_duration_state->addPredecessor(1);
  }
};

TEST_F(AGHMM, ShouldFindBestPathUsingViterbiDecoding) {
  Matrix gamma;
  Sequence path;
  Sequence sequence = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  auto labeling = ghmm->decodableEvaluator(sequence)->labeling(Labeling::Method::bestPath);
  ASSERT_THAT(labeling.sequence(), ContainerEq(Sequence{0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2}));

  labeling = ghmm->decodableEvaluator(sequence, true)->labeling(Labeling::Method::bestPath);
  ASSERT_THAT(labeling.sequence(), ContainerEq(Sequence{0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2}));
}

TEST_F(AGHMM, ShouldFindBestPathUsingPosteriorDecoding) {
  Matrix gamma;
  Sequence path;
  Sequence sequence = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  auto labeling = ghmm->decodableEvaluator(sequence)->labeling(Labeling::Method::posteriorDecoding);
  ASSERT_THAT(labeling.sequence(), ContainerEq(Sequence{0, 0, 0, 2, 0, 2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 0, 2, 0, 2, 2, 2}));

  labeling = ghmm->decodableEvaluator(sequence, true)->labeling(Labeling::Method::posteriorDecoding);
  ASSERT_THAT(labeling.sequence(), ContainerEq(Sequence{0, 0, 0, 2, 0, 2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 0, 2, 0, 2, 2, 2}));
}

TEST_F(AGHMM, ShouldReturnTheSameValueForTheForwardAndBackwardAlgorithms) {
  Matrix alpha;
  Matrix beta;
  Sequence sequence = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  ASSERT_THAT(ghmm->forward(sequence, alpha), DoubleNear(ghmm->backward(sequence, beta), 1e-4));
}
