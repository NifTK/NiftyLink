/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkImageMessageHelpersTests_h
#define NiftyLinkImageMessageHelpersTests_h

#include <NiftyLinkTestingMacros.h>

namespace niftk
{

/**
* \class NiftyLinkImageMessageHelpersTests
* \brief Tests for classes in NiftyLinkImageMessageHelpers.h
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a> framework.
*
* This class is for developers to read. Comments in this header file should be brief. If you want to
* describe the functionality of the method you are testing, put the description in the header file
* of the real class, not in this test harness. Developers are expected to be able to read the .cxx file.
*/
class NiftyLinkImageMessageHelpersTests: public QObject
{
  Q_OBJECT

private slots:

  /**
   * \brief Tests NiftyLinkImageMessageHelpers::SetQImage and NiftyLinkImageMessageHelpers::GetQImage with grey scale image.
   *
   * Spec:
   *   - Load a grey scale image using QImage.load().
   *   - Set it into an image message.
   *   - Get it from an image message.
   *   - Should be the same result as you started with.
   */
  void SetGetQImageGreyScaleTest();

  /**
   * \brief As SetGetQImageGreyScaleTest(), but with 4 channel image.
   *
   * Spec:
   *   - Load a 4 channel (RGBA) image using QImage.load().
   *   - Set it into an image message.
   *   - Get it from an image message.
   *   - Should be the same result as you started with.
   */
  void SetGetQImage4ChannelTest();

  /**
   * \brief Tests the ability to set/get the matrix on an image.
   *
   * Spec:
   *   - Create matrix
   *   - Set it onto the message
   *   - Get it from the message
   *   - Output matrix should be the same as the one you started with.
   */
  void SetGetMatrixTest();

  /**
   * \brief Tests, NiftyLinkImageMessageHelpers::SaveImage and LoadImage.
   *
   * Spec:
   *   - Generate an RGBA test image as QImage.
   *   - Write to temporary folder.
   *   - Read it back in.
   *   - Convert to QImage
   *   - Should be the same as the result you started with.
   */
  void ReadWriteImageTest();

};

} // end namespace

#endif // NiftyLinkImageMessageHelpersTests_h
