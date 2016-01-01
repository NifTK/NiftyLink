/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkImageMessageHelpersTests.h"
#include <NiftyLinkUtils.h>
#include <NiftyLinkImageMessageHelpers.h>
#include <igtl_image.h>
#include <igtlMath.h>
#include <math.h>
#include <exception>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkImageMessageHelpersTests::SetGetQImageGreyScaleTest()
{
  // Load image, JUST using Qt.

  // Qt4 returns QImage::Format_Indexed8
  // Qt5 returns QImage::Format_Grayscale8, which doesn't exist in Qt4.
  QImage i1(":/NiftyLink/UCL_LOGO.pgm");

  QImage i2;

  igtl::ImageMessage::Pointer msg = igtl::ImageMessage::New();
  niftk::SetQImage(i1, msg);
  niftk::GetQImage(msg, i2);

  QVERIFY(msg->GetPackSize() == 1004*297*1*1 + 72 + 58);
  QVERIFY(i1 == i2);
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessageHelpersTests::SetGetQImage4ChannelTest()
{
  // Load image, JUST using Qt.
  QImage i1(":/NiftyLink/UCL_LOGO.tif");

  // Qt4 returns QImage::Format_ARGB32
  // Qt5 returns QImage::Format_ARGB32_Premultiplied
  // So here, we force it.
  i1 = i1.convertToFormat(QImage::Format_ARGB32);

  QImage i2;

  igtl::ImageMessage::Pointer msg = igtl::ImageMessage::New();
  niftk::SetQImage(i1, msg);
  niftk::GetQImage(msg, i2);

  QVERIFY(msg->GetPackSize() == 1004*297*4*1 + 72 + 58);
  QVERIFY(i1 == i2);
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessageHelpersTests::SetGetMatrixTest()
{
  igtl::Matrix4x4 mat1;
  igtl::IdentityMatrix(mat1);

  igtl::Matrix4x4 mat2;
  igtl::IdentityMatrix(mat2);

  float q[4] = {0.5, 0.5, 0.4, 2};
  igtl::QuaternionToMatrix(q, mat1);
  mat1[0][3] = 1;
  mat1[1][3] = 2;
  mat1[2][3] = 3;

  igtl::ImageMessage::Pointer msg = igtl::ImageMessage::New();
  msg->SetMatrix(mat1);
  msg->GetMatrix(mat2);

  for (int r = 0; r < 4; r++)
  {
    for (int c = 0; c < 4; c++)
    {
      QVERIFY(fabs(mat2[r][c] - mat1[r][c]) < 0.0001);
    }
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkImageMessageHelpersTests::ReadWriteImageTest()
{
  igtl::ImageMessage::Pointer msg1 = igtl::ImageMessage::New();
  igtl::ImageMessage::Pointer msg2 = igtl::ImageMessage::New();
  niftk::InitialiseImageWithRandomData(msg1);

  QImage i1;
  QImage i2;

  niftk::GetQImage(msg1, i1);

  QString tmpDir = niftk::GetTempDirectoryPath();
  QString fileName = niftk::AppendPathSeparator(tmpDir) + QString("ReadWriteGreyscaleTest.png");

  niftk::SaveImage(msg1, fileName);
  niftk::LoadImage(fileName, msg2);

  niftk::GetQImage(msg2, i2);
  QVERIFY(i1 == i2);
}

} // end namespace

NIFTYLINK_QTEST_MAIN( niftk::NiftyLinkImageMessageHelpersTests )
