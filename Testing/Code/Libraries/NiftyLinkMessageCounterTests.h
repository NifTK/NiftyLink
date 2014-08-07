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
 * \brief Tests for classes in NiftyLinkMessageCounter and NiftyLinkMessageStatsContainer.
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
   * \brief QMap<QString, quint64> must be a value type with copy/assignment semantics.
   *
   * Spec:
   *   - Create QMap map1, populate.
   *   - Create a copy contructed list map2 from map1, check map2 == map1.
   *   - Create QMap map3, empty, check map3 != map1.
   *   - Assign map3 = map1, check map3 == map1.
   */
  void QMapCopyingTest();

  /**
   * \brief Tests NiftyLinkMessageStatsContainer.
   *
   * Spec:
   *   - NiftyLinkMessageStatsContainer starts empty, so check getters return zero.
   *   - Add 2 messages, one IMAGE, one TDATA, check two entries in GetNumberOfMessagesByType, check total bytes, number received etc.
   *   - ResetPeriod should reset variables that relevant to timing points, but not the all time total.
   *   - ResetAll should reset everything back to zero.
   */
  void BasicStatsCountingTestCase();
};

} // end namespace niftk

#endif // NiftyLinkMessageCounterTests_h
