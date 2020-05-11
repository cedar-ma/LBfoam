/*
"#############################################################################"
"                                                                             "
"  LBfoam: An open-source software package for the simulation of foaming      "
"  using the Lattice Boltzmann Method               	                        "
"  Copyright (C) 2020 Mohammadmehdi Ataei                                     "
"  m.ataei@mail.utoronto.ca                                                   "
"                                                                             "
"  This file is part of LBfoam.                                               "
"                                                                             "
"  LBfoam is free software: you can redistribute it and/or modify it under    "
"  the terms of the GNU Affero General Public License as published by the     "
"  Free Software Foundation version 3.                                        "
"                                                                             "
"  LBfoam is distributed in the hope that it will be useful, but WITHOUT ANY  "
"  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  "
"  FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for   "
"  more details.                                                              "
"                                                                             "
"  You should have received a copy of the GNU Affero General Public License   "
"  along with this Program. If not, see <http://www.gnu.org/licenses/>.       "
"                                                                             "
"#############################################################################"
"                                                                             "
"  Author:         Mohammadmehdi Ataei, 2020                                  "
"                                                                             "
"#############################################################################"
"                                                                             "
"  Parts of the LBfoam code that originate from Palabos are distributed       "
"  under the terms of the AGPL 3.0 license with the following copyright       "
"  notice:                                                                    "
"                                                                             "
"  This file is part of the Palabos library.                                  "
"                                                                             "
"  Copyright (C) 2011-2017 FlowKit Sarl                                       "
"  Route d'Oron 2                                                             "
"  1010 Lausanne, Switzerland                                                 "
"  E-mail contact: contact@flowkit.com                                        "
"                                                                             "
"  The most recent release of Palabos can be downloaded at                    "
"  <http://www.palabos.org/>                                                  "
"                                                                             "
"  The library Palabos is free software: you can redistribute it and/or       "
"  modify it under the terms of the GNU Affero General Public License as      "
"  published by the Free Software Foundation, either version 3 of the         "
"  License, or (at your option) any later version.                            "
"                                                                             "
"  The library is distributed in the hope that it will be useful,             "
"  but WITHOUT ANY WARRANTY; without even the implied warranty of             "
"  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               "
"  GNU Affero General Public License for more details.                        "
"                                                                             "
"  You should have received a copy of the GNU Affero General Public License   "
"  along with this program.  If not, see <http://www.gnu.org/licenses/>.      "
"                                                                             "
"                                                                             "
"                                                                             "
"#############################################################################"
*/
#include "core/array.h"

namespace plb {
namespace lbfoam {

template <typename T>
class rayTracer3D {
 public:
  rayTracer3D(plint iX, plint iY, plint iZ, Array<T, 3> normal) {
    tMaxX = std::abs(halfLength / normal[0]);
    tMaxY = std::abs(halfLength / normal[1]);
    tMaxZ = std::abs(halfLength / normal[2]);
    tDeltaX = std::abs(fullLength / normal[0]);
    tDeltaY = std::abs(fullLength / normal[1]);
    tDeltaZ = std::abs(fullLength / normal[2]);

    stepX = normal[0] >= 0.0 ? 1 : -1;
    stepY = normal[1] >= 0.0 ? 1 : -1;
    stepZ = normal[2] >= 0.0 ? 1 : -1;

    nextCell[0] = iX;
    nextCell[1] = iY;
    nextCell[2] = iZ;
  }
  ~rayTracer3D() {}

  Array<T, 3> findNextCell() {
    if ((tMaxX < tMaxY || std::isinf(tMaxY)) &&
        tMaxX != -std::numeric_limits<T>::infinity()) {
      if ((tMaxX < tMaxZ || std::isinf(tMaxZ)) &&
          tMaxX != -std::numeric_limits<T>::infinity()) {
        tMaxX += tDeltaX;
        nextCell[0] += stepX;
      } else {
        nextCell[2] += stepZ;
        tMaxZ += tDeltaZ;
      }
    } else {
      if ((tMaxY < tMaxZ || std::isinf(tMaxZ)) &&
          tMaxY != -std::numeric_limits<T>::infinity()) {
        nextCell[1] += stepY;
        tMaxY += tDeltaY;
      } else {
        nextCell[2] += stepZ;
        tMaxZ = tMaxZ + tDeltaZ;
      }
    }

    return nextCell;
  }

 private:
  const T halfLength = (T)0.5;
  const T fullLength = (T)1.;

  T tMaxX, tMaxY, tMaxZ;
  T tDeltaX, tDeltaY, tDeltaZ;

  plint stepX, stepY, stepZ;
  Array<T, 3> nextCell;
};

}  // namespace lbfoam
}  // namespace plb
