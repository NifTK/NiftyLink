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
 * \file NiftyLinkTransformMessageHelpersTests.h
 * \class NiftyLinkTransformMessageHelpersTests
 * \brief Tests for classes in NiftyLinkTransformMessageHelpers.h
 *
 * This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a>
 * framework. The test harness also provides documentary evidence of the specification.
 * PLEASE keep comments in this class (a) correct, (b) concise and (c) complete.
 */
class NiftyLinkTransformMessageHelpersTests: public QObject
{
  Q_OBJECT

private slots:

  /**
   * \brief Tests Setting and Getting an igtl::TransformMessage.
   *
   * Spec:
   *   - Load a transform.
   *   - Set it into an igtl::Transform message
   *   - Get it from an igtl::Transform message
   *   - Should be the same result as you started with
   */
  void SetGetTransformTest();

};

} // end namespace

#endif // NiftyLinkTransformMessageHelpersTests_h
