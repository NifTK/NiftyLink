/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkUtils_h
#define NiftyLinkUtils_h

#include "NiftyLinkCommonWin32ExportHeader.h"
#include <NiftyLinkMessageContainer.h>

#include <igtlMath.h>
#include <igtlTimeStamp.h>
#include <igtlStringMessage.h>

#include <QString>

/**
 * \namespace niftk
 * \brief The niftk namespace should be used for all files in the NiftyLink/Code and NiftyLink/Testing folders.
 *
 * \namespace QsLogging
 * \brief Namespace used in the QsLogging package, included within NiftyLink/Code/Libraries/Common folder.
 */

/**
 * \file NiftyLinkUtils.h
 * \brief Helper methods for stats, test data, IP validation etc.
 */
namespace niftk
{

/// \brief Simple function which validates if an IP address is valid or not,
/// by checking for 4 numbers [0-255], each separated by a single dot.
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool ValidateIp(const QString &inputIP);

/// \brief This function returns the local IP address.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetLocalHostAddress(void);

/// \brief This function resolves a given hostname to IP.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString ResolveHostName(const QString &input);

/// \brief For testing, this function creates a random 4x4 test matrix.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix);

/// \brief Assuming input is 16 doubles, copies to output.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CopyMatrix(double *input, igtl::Matrix4x4& output);

/// \brief For testing, this function creates a test message containing a configurable number of tracking matrix elements.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTestTrackingDataMessage(igtl::TimeStamp::Pointer& timeStamp, int matricesPerMessage);

/// \brief Returns the difference in nanoseconds.
/// \param timeA must be not-NULL
/// \param timeB must be not-NULL
extern "C++" NIFTYLINKCOMMON_WINEXPORT igtlUint64 GetDifferenceInNanoSeconds(igtl::TimeStamp* timeA, igtl::TimeStamp* timeB);

/// \brief Used to calculate stats of latency, this method computes the mean of a list of igtlUint64.
extern "C++" NIFTYLINKCOMMON_WINEXPORT double CalculateMean(const QList<igtlUint64>& list);

/// \brief Used to calculate stats of latency, this method computes the standard deviation of a list of igtlUint64.
extern "C++" NIFTYLINKCOMMON_WINEXPORT double CalculateStdDev(const QList<igtlUint64>& list);

/// \brief Used to calculate stats of latency, this method computes the maximum of a list of igtlUint64.
extern "C++" NIFTYLINKCOMMON_WINEXPORT igtlUint64 CalculateMax(const QList<igtlUint64>& list);

} // end namespace niftk

#endif // NiftyLinkUtils_h
