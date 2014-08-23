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
 * \file NiftyLinkImageMessageHelpersTests.h
 * \class NiftyLinkImageMessageHelpersTests
 * \brief Tests for classes in NiftyLinkImageMessageHelpers.h
 *
 * This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a>
 * framework. The test harness also provides documentary evidence of the specification.
 * PLEASE keep comments in this class (a) correct, (b) concise and (c) complete.
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
