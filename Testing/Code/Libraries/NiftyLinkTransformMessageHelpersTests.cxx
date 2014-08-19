/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkTransformMessageHelpersTests.h"
#include <NiftyLinkTransformMessageHelpers.h>
#include <NiftyLinkUtils.h>
#include <QDebug>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkTransformMessageHelpersTests::SetGetTransformTest()
{

  igtl::Matrix4x4 mat1;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      mat1[i][j] = i*j;
    }
  }
  mat1[3][0] = 0;
  mat1[3][1] = 0;
  mat1[3][2] = 0;
  mat1[3][3] = 1;

  igtl::TransformMessage::Pointer msg = igtl::TransformMessage::New();
  msg->SetMatrix(mat1);

  igtl::Matrix4x4 mat2;
  msg->GetMatrix(mat2);

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      double expected = mat1[r][c];
      double actual = mat2[r][c];
      QVERIFY(expected == actual);
    }
  }
}

} // end namespace

QTEST_MAIN( niftk::NiftyLinkTransformMessageHelpersTests )
