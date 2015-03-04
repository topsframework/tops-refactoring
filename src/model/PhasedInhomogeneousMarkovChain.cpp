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

// ToPS headers
#include "PhasedInhomogeneousMarkovChain.hpp"

namespace tops {
namespace model {

PhasedInhomogeneousMarkovChainPtr PhasedInhomogeneousMarkovChain::make(
    std::vector<VariableLengthMarkovChainPtr> vlmcs) {
  return PhasedInhomogeneousMarkovChainPtr(
    new PhasedInhomogeneousMarkovChain(vlmcs));
}

PhasedInhomogeneousMarkovChain::PhasedInhomogeneousMarkovChain(
    std::vector<VariableLengthMarkovChainPtr> vlmcs)
    : InhomogeneousMarkovChain(vlmcs) {
}

PhasedInhomogeneousMarkovChainPtr
  PhasedInhomogeneousMarkovChain::trainInterpolatedPhasedMarkovChain(
    std::vector<Sequence> training_set,
    unsigned int alphabet_size,
    unsigned int order,
    unsigned int nphases,
    double pseudo_counts,
    std::vector<double> weights,
    ProbabilisticModelPtr apriori) {

  unsigned int length = nphases;
  std::vector<VariableLengthMarkovChainPtr> vlmcs(length);

  for (unsigned int i = 0; i < length; i++) {
    std::vector<double> positional_weights;
    std::vector<Sequence> positionalSample;
    for (unsigned int j = 0; j < training_set.size(); j++) {
      int nseq = 0;
      while (true) {
        int start = length * nseq - order + i;
        if (start < 0) {
          nseq++;
          continue;
        }
        unsigned int end = length * nseq + i;
        if (end >= training_set[j].size())
          break;
        Sequence s;
        for (unsigned int k = start; k <= end; k++) {
          s.push_back(training_set[j][k]);
          positional_weights.push_back(weights[j]);
        }
        positionalSample.push_back(s);
        nseq++;
      }
    }

    ContextTreePtr tree = ContextTree::make(alphabet_size);

    if (apriori != NULL) {
      tree->initializeCounter(positionalSample, order, 0, positional_weights);
      tree->pruneTreeSmallSampleSize(400);
      tree->normalize(apriori, pseudo_counts);
    } else {
      tree->initializeCounter(positionalSample, order, pseudo_counts,
                              positional_weights);
      tree->pruneTreeSmallSampleSize(400);
      tree->normalize();
    }

    vlmcs[i] = VariableLengthMarkovChain::make(tree);
  }

  return PhasedInhomogeneousMarkovChain::make(vlmcs);
}

double PhasedInhomogeneousMarkovChain::evaluatePosition(const Sequence &s,
                                                        unsigned int i) const {
  return _vlmcs[i % _vlmcs.size()]->evaluatePosition(s, i);
}

Symbol PhasedInhomogeneousMarkovChain::choosePosition(const Sequence &s,
                                                   unsigned int i) const {
  return _vlmcs[i % _vlmcs.size()]->choosePosition(s, i);
}

}  // namespace model
}  // namespace tops
