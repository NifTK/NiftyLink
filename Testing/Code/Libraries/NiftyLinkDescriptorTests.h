/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkDescriptorTests_h
#define NiftyLinkDescriptorTests_h

#include <NiftyLinkTestingMacros.h>

namespace niftk
{

/**
* \class NiftyLinkDescriptorTests
* \brief Tests for classes in file NiftyLinkXMLBuilder.h and classes NiftyLinkClientDescriptor etc.
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a> framework.
*
* This class is for developers to read. Comments in this header file should be brief. If you want to
* describe the functionality of the method you are testing, put the description in the header file
* of the real class, not in this test harness. Developers are expected to be able to read the .cxx file.
*/
class NiftyLinkDescriptorTests: public QObject
{
  Q_OBJECT

private slots:

  /**
   * \brief Tests for niftk::NiftyLinkClientDescriptor, basically setting/getting XML.
   */
  void ClientDescriptorTest();

  /**
   * \brief Tests for niftk::NiftyLinkTrackerClientDescriptor, basically setting/getting XML.
   */
  void TrackerClientDescriptorTest();

  /**
   * \brief Tests for niftk::NiftyLinkCommandDescriptor, basically setting/getting XML.
   */
  void CommandDescriptorTest();
};

} // end namespace niftk

#endif // NiftyLinkDescriptorTests_h
