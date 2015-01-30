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

#include "VariableLengthMarkovChain.hpp"

namespace tops {
namespace model {

VariableLengthMarkovChainPtr VariableLengthMarkovChain::make(tops::ContextTreePtr context_tree) {
  return VariableLengthMarkovChainPtr(new VariableLengthMarkovChain(context_tree));
}

VariableLengthMarkovChain::VariableLengthMarkovChain(tops::ContextTreePtr context_tree) : tops::VariableLengthMarkovChain(context_tree) {
}

VariableLengthMarkovChainPtr VariableLengthMarkovChain::make(ContextTreePtr context_tree) {
  return VariableLengthMarkovChainPtr(new VariableLengthMarkovChain(context_tree));
}

VariableLengthMarkovChain::VariableLengthMarkovChain(ContextTreePtr context_tree) : _context_tree(context_tree) {
}

int VariableLengthMarkovChain::alphabetSize() const {
  return 0;
}

double VariableLengthMarkovChain::evaluatePosition(const Sequence &s, unsigned int i) const {
  ContextTreeNodePtr c = _context_tree->getContext(s,i);
  if (c == NULL)
    return -HUGE;
  else
    return c->getDistribution()->probabilityOf(s[i]);
}

double VariableLengthMarkovChain::evaluateSequence(const Sequence &s, unsigned int begin, unsigned int end) const {
  double p = 0;
  for (int i = begin; i < end; i++)
    p += evaluatePosition(s, i);
  return p;
}

int VariableLengthMarkovChain::choosePosition(const Sequence &s, unsigned int i) const {
  ContextTreeNodePtr c = _context_tree->getContext(s,i);
  if (c == NULL)
    // TODO: ERROR!
    return 0;
  else
    return c->getDistribution()->choose();
}


}
}
