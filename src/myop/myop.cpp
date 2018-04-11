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
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Internal headers
#include "myop/Myop.hpp"
#include "myop/states.hpp"
#include "model/Sequence.hpp"


int main(int argc, char *argv[]) {
  std::string dataset = "dataset";

  if (argc >= 2) {
    dataset = argv[1];
  }

  // Random sequence
  // 1, 3, 1, 2, 0, 1, 3, 0, 2

  // Real gene (AT1G34280)
  tops::model::Sequence sequence {
    3, 1, 3, 0, 1, 2, 3, 0, 0, 3, 2, 3, 0, 2, 0, 2, 2, 0, 0, 2, 0, 0,
    3, 0, 2, 0, 1, 0, 1, 0, 3, 2, 0, 3, 3, 3, 3, 1, 0, 3, 0, 2, 3, 3,
    0, 3, 0, 0, 3, 3, 0, 3, 1, 1, 1, 0, 3, 2, 3, 2, 1, 3, 3, 0, 3, 0,
    3, 3, 1, 0, 0, 0, 1, 1, 3, 3, 2, 0, 3, 1, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 0, 1, 1, 1, 3, 2, 0, 1, 3, 2, 0, 3, 1, 1, 0, 0, 2, 0,
    1, 1, 0, 0, 1, 0, 2, 0, 1, 1, 0, 3, 1, 1, 0, 0, 0, 2, 3, 1, 0, 0,
    3, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 3, 0, 2, 2, 1, 1, 1, 3, 1, 1, 0,
    1, 3, 0, 2, 3, 0, 2, 0, 3, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0,
    0, 3, 2, 1, 3, 3, 3, 0, 1, 1, 3, 0, 0, 0, 2, 0, 1, 3, 3, 0, 3, 3,
    0, 1, 1, 0, 1, 1, 0, 3, 3, 3, 1, 1, 0, 2, 0, 0, 0, 1, 0, 3, 1, 0,
    2, 0, 3, 3, 3, 3, 0, 1, 0, 3, 0, 3, 3, 1, 0, 1, 3, 3, 1, 1, 0, 1,
    3, 0, 3, 3, 2, 0, 3, 2, 1, 1, 0, 2, 3, 3, 0, 1, 3, 1, 2, 3, 0, 0,
    2, 3, 3, 2, 0, 3, 1, 0, 3, 0, 3, 1, 0, 1, 0, 0, 2, 2, 0, 3, 3, 1,
    0, 3, 0, 3, 0, 3, 3, 3, 0, 3, 2, 0, 0, 1, 0, 1, 1, 2, 2, 0, 3, 0,
    0, 1, 1, 2, 1, 3, 1, 1, 0, 3, 3, 3, 0, 3, 1, 2, 0, 0, 3, 3, 3, 3,
    0, 3, 3, 1, 3, 1, 1, 3, 1, 0, 1, 3, 3, 1, 2, 3, 0, 0, 1, 3, 3, 3,
    0, 3, 0, 0, 0, 3, 1, 1, 2, 3, 1, 0, 1, 0, 0, 0, 3, 0, 3, 1, 3, 3,
    1, 0, 3, 1, 2, 3, 3, 1, 0, 3, 3, 0, 2, 1, 0, 0, 1, 3, 3, 2, 3, 3,
    3, 2, 1, 3, 3, 1, 1, 1, 0, 3, 1, 0, 0, 0, 0, 3, 0, 3, 0, 3, 3, 1,
  };

  std::vector<tops::model::Probability> extrinsic_contribuition;

  if (argc >= 3) {
    string extrinsicMethod = argv[2];
    if(extrinsicMethod.compare("augustus") == 0){
      ExtrinsicInformation extrinsic_info;
      auto augustus = std::make_shared<Augustus>(sequence.size());
      extrinsic_info.extrinsicTechnique(augustus);
      extrinsic_contribuition
        = extrinsic_info.extrinsicTechnique()->makeContribuition();
    }
  } else {
    ExtrinsicInformation extrinsic_info;
    auto nohints = std::make_shared<NoHints>(sequence.size());
    extrinsic_info.extrinsicTechnique(nohints);
    extrinsic_contribuition
      = extrinsic_info.extrinsicTechnique()->makeContribuition();
  }

  tops::myop::Myop myop(dataset);

  auto prediction = myop.predict(sequence, extrinsic_contribuition);

  for (auto symbol : prediction) {
    std::cout << tops::myop::state_names[symbol] << " ";
  }

  std::cout << std::endl;

  return 0;
}
