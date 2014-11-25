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
* \class NiftyLinkUtilsTests
* \brief Tests for classes in NiftyLinkUtils.h
*
* This test harness uses the <a href="http://qt-project.org/doc/qt-4.8/qtestlib-manual.html">QTestLib</a> framework.
*
* This class is for developers to read. Comments in this header file should be brief. If you want to
* describe the functionality of the method you are testing, put the description in the header file
* of the real class, not in this test harness. Developers are expected to be able to read the .cxx file.
*/
class NiftyLinkUtilsTests: public QObject
{
  Q_OBJECT

private slots:
  /**
   * \brief Tests NiftyLinkUtils::GetDifferenceInNanoSeconds(), and also general time stamping tests.
   *
   * The reason for this test is that OpenIGTLink has a fixed point representation of a fraction
   * of a second. See <a href="http://openigtlink.org/protocols/v2_timestamp.html">this</a>. So,
   * if you convert between timestamps, the fraction of a second will change due to rounding error.
   * So, the NifTK project added the SetTimeInNanoseconds() and GetTimeStampInNanoseconds().
   * So, these are tested here, and should result in consistent numbers of nanoseconds.
   */
  void GetDifferenceInNanoSecondsTest();

  /**
   * \brief Tests NiftyLinkUtils::ValidateIp(), checking for 4 dot separated integers between [0-255].
   */
  void ValidateIpTest();

  /**
   * \brief Tests NiftyLinkUtils::GetLocalHostAddress(), returning the machine's IP address, and NOT the loopback 127.0.0.1 address.
   *
   * Note:
   *   - How do we make Qt fail to return a network address?
   *   - As a compromise, we just check for UNKNOWN, or a valid IP address in that order.
   */
  void GetLocalHostAddressTest();

  /**
   * \brief Tests NiftyLinkUtils::ResolveHostName(), converting a name to an IP address via DNS lookup.
   */
  void ResolveHostNameTest();

  /**
   * \brief Tests NiftyLinkUtils::CalculateMean(), returning the mean average of a QList<igtlUint64>.
   */
  void CalculateMeanTest();

  /**
   * \brief Tests NiftyLinkUtils::CalculateStdDev(), returning the <a href="http://en.wikipedia.org/wiki/Standard_deviation">corrected sample standard deviation</a> of a QList<igtlUint64>.
   */
  void CalculateStdDevTest();

  /**
   * \brief Tests NiftyLinkUtils::CalculateMax(), returning the maximum of a QList<igtlUint64>.
   */
  void CalculateMaxTest();

  /**
   * \brief Tests NiftyLinkUtils::IsCloseEnoughTo() and NiftyLinkUtils::IsCloseEnoughToZero()
   */
  void CalculateCloseEnoughToTest();

  /**
   * \brief Tests NiftyLinkUtils::IsCloseEnoughTo for matrices.
   */
  void CalculateCloseEnoughToMatrixTest();

  /**
   * \brief Tests NiftyLinkUtils::CopyMatrix() to copy an array of doubles into a matrix.
   */
  void CopyMatrixTest();

  /**
   * \brief Tests NiftyLinkUtils::GetWritableDirectoryPath() to check we get a valid directory and can append a file name if provided.
   */
  void GetWritableDirectoryTest();

};

} // end namespace

#endif // NiftyLinkUtilsTests_h
