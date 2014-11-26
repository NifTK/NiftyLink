/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NLTest_Data_h
#define NLTest_Data_h

#include "igtlMath.h"

static igtl::Matrix4x4 dummyTransformMatrix =
{
  { -1, 0, 0, 50},
  {  0, 0.15, 0.98, 0},
  { 0, 0.98, -0.15, 50},
  { 0, 0, 0, 1}
};

#endif // NLTEST_DATA_H
