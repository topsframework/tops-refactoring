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

#include "model/TargetModel.hpp"

namespace tops {
namespace model {

TargetModelPtr TargetModel::make(int alphabet_size) {
  return TargetModelPtr(new TargetModel(alphabet_size));
}

TargetModel::TargetModel(int alphabet_size)
    : DiscreteIIDModel(
      std::vector<double>(alphabet_size, log(1.0/alphabet_size))) {
}

double TargetModel::evaluatePosition(const Sequence &s, unsigned int i) const {
  auto iid = DiscreteIIDModel::trainML({s}, alphabetSize());
  return iid->evaluatePosition(s, i);
}

double TargetModel::evaluateSequence(const Sequence &s,
                                     unsigned int begin,
                                     unsigned int end) const {
  std::vector<Symbol> subseq;
  for (unsigned int i = begin; i < end; i++)
    subseq.push_back(s[i]);

  auto iid = DiscreteIIDModel::trainML({subseq}, alphabetSize());

  return iid->evaluateSequence(s, begin, end);
}

}  // namespace model
}  // namespace tops