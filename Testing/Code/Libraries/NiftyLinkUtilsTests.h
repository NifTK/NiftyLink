/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkUtilsTests_h
#define NiftyLinkUtilsTests_h

#include <NiftyLinkTestingMacros.h>

namespace niftk
{

/**
 * \file NiftyLinkUtilsTests.h
 * \class NiftyLinkUtilsTests
 * \brief Tests for classes in NiftyLinkUtils.h
 *
 * This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a>
 * framework. The test harness also provides documentary evidence of the specification.
 * PLEASE keep comments in this class (a) correct, (b) concise and (c) complete.
 */
class NiftyLinkUtilsTests: public QObject
{
  Q_OBJECT

private slots:
  /**
   * \brief Tests NiftyLinkUtils::GetDifferenceInNanoSeconds().
   *
   * The reason for this test is that OpenIGTLink has a fixed point representation of a fraction
   * of a second. See <a href="http://openigtlink.org/protocols/v2_timestamp.html">this</a>. So,
   * if you convert between timestamps, the fraction of a second will change due to rounding error.
   * So, the NifTK project added the SetTimeInNanoseconds() and GetTimeStampInNanoseconds().
   * So, these are tested here, and should result in consistent numbers of nanoseconds.
   *
   * Spec:
   *   - Given two time-stamps, the difference should be a positive number of nanoseconds.
   */
  void GetDifferenceInNanoSecondsTest();

  /**
   * \brief Tests NiftyLinkUtils::ValidateIp(), checking for 4 dot separated integers between [0-255].
   *
   * Spec:
   *  - if empty, return false.
   *  - if not 4 numbers, [0-255], return false.
   *  - otherwise true.
   */
  void ValidateIpTest();

  /**
   * \brief Tests NiftyLinkUtils::GetLocalHostAddress(), returning the machine's IP address, and NOT the loopback 127.0.0.1 address.
   *
   * Spec:
   *   - returns a valid IP address.
   *   - should not return 127.0.0.1.
   *   - otherwise (if fails) return "UNKNOWN".
   *
   * Note: the third point is un-tested by this unit test.
   * How do we make Qt fail to return a network address?
   *
   * As a compromise, we just check for UNKNOWN, or a valid IP address in that order.
   */
  void GetLocalHostAddressTest();

  /**
   * \brief Tests NiftyLinkUtils::ResolveHostName(), converting a name to an IP address.
   *
   * Spec:
   *   - if its a valid IP address, return it.
   *   - if its a name that converts to an IP address, return the IP address.
   *   - otherwise return "UNKNOWN".
   */
  void ResolveHostNameTest();

  /**
   * \brief Tests NiftyLinkUtils::CalculateMean(), returning the mean average of a QList<igtlUint64>.
   *
   * Spec:
   *   - if empty list, return zero.
   *   - otherwise return the mean of the QList.
   */
  void CalculateMeanTest();

  /**
   * \brief Tests NiftyLinkUtils::CalculateStdDev(), returning the <a href="http://en.wikipedia.org/wiki/Standard_deviation>corrected sample standard deviation</a> of a QList<igtlUint64>.
   *
   * Spec:
   *   - if empty list, return zero.
   *   - otherwise return the corrected sample standard deviation of the QList.
   */
  void CalculateStdDevTest();

  /**
   * \brief Tests NiftyLinkUtils::CalculateMax(), returning the maximum of a QList<igtlUint64>.
   *
   * Spec:
   *   - if empty list, return zero.
   *   - otherwise return the largest member in the QList.
   */
  void CalculateMaxTest();

  /**
   * \brief Tests NiftyLinkUtils::IsCloseEnoughTo() and NiftyLinkUtils::IsCloseEnoughToZero()
   *
   * Spec:
   *   - if absolute diff between two numbers is strictly less than the tolerance.
   */
  void CalculateCloseEnoughToTest();
};

} // end namespace

#endif // NiftyLinkUtilsTests_h
