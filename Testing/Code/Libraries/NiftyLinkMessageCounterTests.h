/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkMessageCounterTests_h
#define NiftyLinkMessageCounterTests_h

#include <QObject>
#include <QtTest/QtTest>

namespace niftk
{

/**
 * \file NiftyLinkMessageCounterTests.h
 * \class NiftyLinkMessageCounterTests
 * \brief Tests for classes in NiftyLinkMessageCounterTests.
 *
 * This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a>
 * framework. The test harness also provides documentary evidence of the specification.
 * PLEASE keep comments in this class (a) correct, (b) concise and (c) complete.
 */
class NiftyLinkMessageCounterTests: public QObject
{
  Q_OBJECT

private slots:
  /**
   * \brief Tests NiftyLinkMessageCounter.
   */
  void BasicCountingTestCase();
};

} // end namespace niftk

#endif // NiftyLinkMessageCounterTests_h
