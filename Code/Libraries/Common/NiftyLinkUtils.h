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

#include <QsLog.h>

#include <QString>
#include <QPlainTextEdit>
#include <QLabel>

#if defined(_WIN32) && !defined(__CYGWIN__)
  #include <windows.h>
  #include <bcrypt.h>
  #define WINAPI __stdcall

  // Function pointers that will be used for the DLL functions.
  typedef NTSTATUS (__stdcall *FunctionPtr_SETRES) (ULONG, BOOLEAN, PULONG);
  typedef NTSTATUS (__stdcall *FunctionPtr_GETRES) (PULONG, PULONG, PULONG);
#endif

/**
 * \file NiftyLinkUtils.h
 * \brief Helper methods for stats, test data, IP validation etc.
 *
 * \namespace niftk
 * \brief The niftk namespace should be used for all files in the NiftyLink/Code and NiftyLink/Testing folders.
 *
 * \namespace QsLogging
 * \brief Namespace used in the QsLogging package, included within NiftyLink/Code/Libraries/Common folder.
 */

namespace niftk
{

/**
 * \name WindowsSpecificFunctions
 * As few as possible please.
 */
///@{
#if defined(_WIN32) && !defined(__CYGWIN__)
  extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitializeWinTimers();
#endif
///@}

/**
 * \name NiftyLinkHostNameUtils
 * Functions for hostname, IP lookups etc.
 */
///@{

/// \brief Simple function which validates if an IP address is valid or not,
/// by checking for 4 numbers [0-255], each separated by a single dot.
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool ValidateIp(const QString &inputIP);

/// \brief This function returns the local IP address.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetLocalHostAddress(void);

/// \brief This function resolves a given hostname to IP.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString ResolveHostName(const QString &input);

///@}

/**
 * \name NiftyLinkTestingUtils
 * \brief Used mainly for testing, visible in public API just in case anyone wants them.
 */
///@{

/// \brief For testing, this function creates a random 4x4 test matrix.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix);

/// \brief For testing, this function creates a test message containing a configurable number of tracking matrix elements.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTestTrackingDataMessage(igtl::TimeStamp::Pointer& timeStamp, int matricesPerMessage);

///@}


/**
 * \name NiftyLinkMathsUtils
 * \brief Simple mathematical functions used throughout NiftyLink.
 */
///@{

/// \brief Used to calculate stats of latency, this method computes the mean of a list of igtlUint64.
extern "C++" NIFTYLINKCOMMON_WINEXPORT double CalculateMean(const QList<igtlUint64>& list);

/// \brief Used to calculate stats of latency, this method computes the standard deviation (of the sample) of a list of igtlUint64.
extern "C++" NIFTYLINKCOMMON_WINEXPORT double CalculateStdDev(const QList<igtlUint64>& list);

/// \brief Used to calculate stats of latency, this method computes the maximum of a list of igtlUint64.
extern "C++" NIFTYLINKCOMMON_WINEXPORT igtlUint64 CalculateMax(const QList<igtlUint64>& list);

/// \brief Returns true if the absolute difference between valueA and valueB is less than tolerance.
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsCloseEnoughTo(double valueA, double valueB, double tolerance=0.00001);

/// \brief Returns true if valueA is closer than tolerance to zero.
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsCloseEnoughToZero(double valueA, double tolerance=0.00001);

///@}


/**
 * \name NiftyLinkFileUtils
 * \brief Helper functions for file/directory processing.
 */
///@{

/// \brief Tries to create a path name where data can be saved by checking the Desktop,
/// Documents folder, HOME folder, cache folder, or the system temporary directory.
/// \param fileName - if specified, will be concatenated onto the path.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetWritableDirectoryPath(const QString& fileName=QString());

/// \brief Used to get a temporary folder, determined by QDesktopServices.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetTempDirectoryPath();

/// \brief If required will add the path separator.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString AppendPathSeparator(const QString& path);

///@}


/**
 * \name NiftyLinkWidgetUtils
 * \brief Helper functions for pasting data onto QWidget, mainly used by NiftyLink app.
 */
///@{

/// \brief If its a STRING, STATUS, TDATA or TRANSFORM message, updates the output string, otherwise, leaves it alone.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void ExtractTextBasedMessage(const NiftyLinkMessageContainer::Pointer& message, QString& output);

/// \brief If its an IMAGE message, updates the output image, otherwise, leaves it alone.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void ExtractImageMessage(const NiftyLinkMessageContainer::Pointer& message, QImage& output);

///@}


/// \brief Assuming input is 16 doubles, copies to output.
/// \param input array of 16 doubles, row-wise, meaning row 0, left to right, then row 1, left to right, etc.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CopyMatrix(double *input, igtl::Matrix4x4& output);

/// \brief Returns the difference in nanoseconds, as a positive number.
/// \param timeA must be not-NULL, only checked with assert()
/// \param timeB must be not-NULL, only checked with assert()
extern "C++" NIFTYLINKCOMMON_WINEXPORT igtlUint64 GetDifferenceInNanoSeconds(igtl::TimeStamp* timeA, igtl::TimeStamp* timeB);

/// \brief Returns true if the message is a STATUS_OK and false otherwise.
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsKeepAlive(const igtl::MessageBase::Pointer&);

/// \brief Returns true if the message is an igtl::StringMessage containing just the text "STATS".
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsStatsRequest(const igtl::MessageBase::Pointer&);

} // end namespace niftk

#endif // NiftyLinkUtils_h
