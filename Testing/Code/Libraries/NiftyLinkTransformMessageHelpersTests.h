/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTransformMessageHelpersTests_h
#define NiftyLinkTransformMessageHelpersTests_h

#include <NiftyLinkTestingMacros.h>

namespace niftk
{

/**
* \class NiftyLinkTransformMessageHelpersTests
* \brief Tests for classes in NiftyLinkTransformMessageHelpers.h
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a> framework.
*
* This class is for developers to read. Comments in this header file should be brief. If you want to
* describe the functionality of the method you are testing, put the description in the header file
* of the real class, not in this test harness. Developers are expected to be able to read the .cxx file.
*/
class NiftyLinkTransformMessageHelpersTests: public QObject
{
  Q_OBJECT

private slots:

  /**
   * \brief Tests Setting and Getting an igtl::Matrix on an igtl::TransformMessage.
   */
  void SetGetMatrixTest();

  /**
   * \brief Create random matrix, make sure its set on message correctly.
   */
  void RandomInitialisationTest();

  /**
   * \brief Tests NiftyLinkTransformMessageHelpers::CreateTransformMessage.
   */
  void CreateTransformMessageTest();

};

} // end namespace

#endif // NiftyLinkTransformMessageHelpersTests_h
