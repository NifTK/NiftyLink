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

#include <NiftyLinkTestingMacros.h>

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
   * \brief QMap<QString, quint64> must be a Value Type with copy and assignment semantics.
   *
   * We believe it should be, as part of the spec of QMap, but its worth double checking!
   *
   * Spec:
   *   - Create QMap map1, populate.
   *   - Create a copy contructed list map2 from map1, check map2 == map1.
   *   - Create QMap map3, empty, check map3 != map1.
   *   - Assign map3 = map1, check map3 == map1.
   */
  void QMapCopyingTest();

  /**
   * \brief NiftyLinkMessageStatsContainer must be a Value Type with copy and assignment semantics.
   *
   * Spec:
   *   - Create NiftyLinkMessageStatsContainer container1, populate.
   *   - Create a copy constructed NiftyLinkMessageStatsContainer container2 from container1, check container2 == container1.
   *   - Create NiftyLinkMessageStatsContainer container3, default, check container3 != container1.
   *   - Assign container3 = container1, check container3 == container1.
   */
  void StatsContainerCopyAssignmentTest();

  /**
   * \brief Tests NiftyLinkMessageStatsContainer.
   *
   * Spec:
   *   - NiftyLinkMessageStatsContainer starts empty, so check all getters return zero.
   *   - Add 2 messages, one STATUS, one STRING, check two entries in GetNumberOfMessagesByType, check total bytes, number received etc.
   *   - Check conversions between nanoseconds, milliseconds and seconds (see method descriptions).
   *   - ResetPeriod should reset variables that are relevant to timing points, but not the all time totals in GetTotalBytesReceived() and GetTotalNumberMessagesReceived().
   *   - ResetAll should reset everything back to zero, including the all time totals in GetTotalBytesReceived() and GetTotalNumberMessagesReceived().
   */
  void BasicStatsContainerCountingTest();

  /**
   * \brief Tests NiftyLinkMessageCounter.
   *
   * Spec:
   *   - Set/Get just set the threshold member variable (m_NumberMessageReceivedThreshold).
   *   - After construction, GetNumberOfMessagesSinceClear() and GetTotalNumberOfMessages() returns 0.
   *   - Adding two messages, and GetNumberOfMessagesSinceClear() returns 2 and GetTotalNumberOfMessages() returns 2.
   *   - Call OnClear() and GetNumberOfMessagesSinceClear() returns 0 and GetTotalNumberOfMessages() returns 2.
   *   - If a message has a negative latency, it should be ignored from statistics.
   */
  void BasicStatsCounterTest();

};

} // end namespace niftk

#endif // NiftyLinkMessageCounterTests_h
