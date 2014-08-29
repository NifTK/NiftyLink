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
#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkTransformMessageHelpersTests::SetGetMatrixTest()
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


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessageHelpersTests::RandomInitialisationTest()
{
  igtl::Matrix4x4 mat1;
  igtl::Matrix4x4 mat2;

  niftk::CreateRandomTransformMatrix(mat1);

  igtl::TransformMessage::Pointer msg1 = igtl::TransformMessage::New();

  niftk::InitialiseTransformWithTestData(mat1, msg1);
  msg1->GetMatrix(mat2);

  std::cerr << "Retrieved matrix:" << std::endl << GetMatrixAsString(msg1).toStdString() << std::endl;

  QVERIFY(niftk::IsCloseEnoughTo(mat1, mat2));
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessageHelpersTests::CreateTransformMessageTest()
{
  igtl::Matrix4x4 mat1;
  niftk::CreateRandomTransformMatrix(mat1);

  NiftyLinkMessageContainer::Pointer msg1 = CreateTransformMessage("TestDevice", "TestHost", 1234, mat1);
  assert(msg1);
  assert(msg1->GetMessage());

  igtl::TransformMessage::Pointer message = dynamic_cast<igtl::TransformMessage*>(msg1->GetMessage().GetPointer());
  assert(message);

  igtl::Matrix4x4 mat2;
  message->GetMatrix(mat2);

  std::cerr << "Retrieved matrix:" << std::endl << GetMatrixAsString(message).toStdString() << std::endl;

  QVERIFY(niftk::IsCloseEnoughTo(mat1, mat2));

  double testData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  CopyMatrix(testData, mat1);

  msg1 = CreateTransformMessage("TestDevice", "TestHost", 1234, testData);
  assert(msg1);
  assert(msg1->GetMessage());
  message = dynamic_cast<igtl::TransformMessage*>(msg1->GetMessage().GetPointer());
  assert(message);
  message->GetMatrix(mat2);

  std::cerr << "Retrieved matrix:" << std::endl << GetMatrixAsString(message).toStdString() << std::endl;

  QVERIFY(niftk::IsCloseEnoughTo(mat1, mat2));

}

} // end namespace

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkTransformMessageHelpersTests )
