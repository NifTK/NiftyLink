/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTrackingDataMessageHelpersTests_h
#define NiftyLinkTrackingDataMessageHelpersTests_h

#include <NiftyLinkTestingMacros.h>

namespace niftk
{

/**
* \class NiftyLinkTrackingDataMessageHelpersTests
* \brief Tests for classes in NiftyLinkTrackingDataMessageHelpers.h
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a> framework.
*
* This class is for developers to read. Comments in this header file should be brief. If you want to
* describe the functionality of the method you are testing, put the description in the header file
* of the real class, not in this test harness. Developers are expected to be able to read the .cxx file.
*/
class NiftyLinkTrackingDataMessageHelpersTests: public QObject
{
  Q_OBJECT

private slots:

  /**
   * \brief Tests Setting and Getting an igtl::TrackingDataMessage.
   *
   * Spec:
   *   - Create a transform
   *   - Set it into an tracking data message
   *   - Get it from an tracking data message
   *   - Should be the same result as you started with
   */
  void SetGetTrackingDataTest();

};

} // end namespace

#endif // NiftyLinkTrackingDataMessageHelpersTests_h
