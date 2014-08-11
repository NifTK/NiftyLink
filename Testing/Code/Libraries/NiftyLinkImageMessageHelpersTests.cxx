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
#include <QDebug>
#include <igtl_image.h>

namespace niftk
{

//-----------------------------------------------------------------------------
void NiftyLinkImageMessageHelpersTests::SetGetQImageGreyScaleTest()
{
  // Load image, JUST using Qt.
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
  QImage i2;

  igtl::ImageMessage::Pointer msg = igtl::ImageMessage::New();
  niftk::SetQImage(i1, msg);
  niftk::GetQImage(msg, i2);

  QVERIFY(msg->GetPackSize() == 1004*297*4*1 + 72 + 58);
  QVERIFY(i1 == i2);
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

QTEST_MAIN( niftk::NiftyLinkImageMessageHelpersTests )
