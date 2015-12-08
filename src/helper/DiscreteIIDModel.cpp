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
#include "helper/DiscreteIIDModel.hpp"
#include "helper/Random.hpp"

#include "model/Probability.hpp"

namespace tops {
namespace helper {

model::DiscreteIIDModelPtr generateRandomIIDModel(int alphabet_size) {
  std::vector<model::Probability> counts;
  for (int i = 0; i < alphabet_size; i++)
    counts.push_back(generateRandomInteger(alphabet_size));
  return model::DiscreteIIDModel::make(counts);
}

model::DiscreteIIDModelPtr createFairCoinIIDModel() {
  std::vector<model::Probability> probabilities{ log(0.5), log(0.5) };
  return model::DiscreteIIDModel::make(probabilities);
}

model::DiscreteIIDModelPtr createLoadedCoinIIDModel() {
  std::vector<model::Probability> probabilities{ log(0.2), log(0.8) };
  return model::DiscreteIIDModel::make(probabilities);
}

model::DiscreteIIDModelPtr createDNAModel() {
  std::vector<model::Probability> probabilities{
    log(0.1), log(0.3), log(0.4), log(0.2)
  };
  return model::DiscreteIIDModel::make(probabilities);
}

}  // namespace helper
}  // namespace tops
