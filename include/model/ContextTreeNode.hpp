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

#ifndef TOPS_MODEL_CONTEXT_TREE_NODE_
#define TOPS_MODEL_CONTEXT_TREE_NODE_

#include <memory>
#include <map>

#include "model/DiscreteIIDModel.hpp"

namespace tops {
namespace model {

class ContextTreeNode;
typedef std::shared_ptr<ContextTreeNode> ContextTreeNodePtr;

class ContextTreeNode {
 public:
  static ContextTreeNodePtr make(DiscreteIIDModelPtr distribution);

  int alphabetSize() const;

  bool isLeaf();
  ContextTreeNodePtr addChild(int symbol, DiscreteIIDModelPtr distribution);
  ContextTreeNodePtr getChild(int symbol);
  DiscreteIIDModelPtr getDistribution();
 private:
  explicit ContextTreeNode(DiscreteIIDModelPtr distribution);

  bool _leaf;
  std::map<int, ContextTreeNodePtr> _children;
  DiscreteIIDModelPtr _distribution;
};

}  // namespace model
}  // namespace tops

#endif  // TOPS_MODEL_CONTEXT_TREE_NODE_
