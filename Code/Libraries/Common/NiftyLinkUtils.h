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

#include <QString>

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

/**
* \brief Validate inputIP is 4 integers [0-255], each separated by a single dot.
* \return true if valid and false otherwise
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool ValidateIp(const QString &inputIP);

/**
* \brief Returns the machine IP address, not the local loopback address.
* \return valid machine IP address, (not 127.0.0.1), or "UNKNOWN" if fails.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetLocalHostAddress(void);

/**
* \brief Resolves a given hostname to an IP address.
* \param input if the input is a valid IP address, will return that,
* otherwise will do DNS lookup to convert to a valid IP address.
* \return valid IP addres or "UNKNOWN".
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString ResolveHostName(const QString &input);

///@}

/**
* \name NiftyLinkTestingUtils
* \brief Used mainly for testing, visible in public API just in case anyone wants them.
*/
///@{

/**
* \brief Populates a 4x4 matrix with random rigid body transform.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix);

///@}


/**
* \name NiftyLinkMathsUtils
* \brief Simple mathematical functions used throughout NiftyLink.
*/
///@{

/**
* \brief Computes the mean of a list of igtlUint64, used when calculating latency stats.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT double CalculateMean(const QList<igtlUint64>& list);

/**
* \brief Computes the standard deviation (of the sample) of a list of igtlUint64, used when calculating latency stats.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT double CalculateStdDev(const QList<igtlUint64>& list);

/**
* \brief Computes the maximum of a list of igtlUint64, used when calculating latency stats.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT igtlUint64 CalculateMax(const QList<igtlUint64>& list);

/**
* \brief Returns true if the absolute difference between valueA and valueB is less than tolerance.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsCloseEnoughTo(double valueA, double valueB, double tolerance=0.00001);

/**
* \brief Returns true if valueA is closer than tolerance to zero.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsCloseEnoughToZero(double valueA, double tolerance=0.00001);

/**
* \brief Returns true if matrix a is within tolerance of matrix b.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsCloseEnoughTo(const igtl::Matrix4x4& a, const igtl::Matrix4x4& b, double tolerance=0.00001);

///@}


/**
* \name NiftyLinkFileUtils
* \brief Helper functions for file/directory processing.
*/
///@{

/**
* \brief Tries to create a path name where data can be saved by checking the Desktop,
* Documents folder, HOME folder, cache folder, or the system temporary directory.
* \param fileName - if specified, will be concatenated onto the path.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetWritableDirectoryPath(const QString& fileName=QString());

/**
* \brief Used to get a temporary folder, determined by QDesktopServices.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetTempDirectoryPath();

/**
* \brief If path does not end in the seperator, this method adds it to the returned string, otherwise, it just returns path.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString AppendPathSeparator(const QString& path);

///@}


/**
* \name NiftyLinkWidgetUtils
* \brief Helper functions for pasting data onto QWidget, mainly used by NiftyLink app.
*/
///@{

/**
* \brief If message is a STRING, STATUS, TDATA or TRANSFORM message, updates the output string, otherwise, leaves it alone.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT void ExtractTextBasedMessage(const NiftyLinkMessageContainer::Pointer& message, QString& output);

/**
* \brief If message is an IMAGE message, updates the output image, otherwise, leaves it alone.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT void ExtractImageMessage(const NiftyLinkMessageContainer::Pointer& message, QImage& output);

///@}


/**
* \brief Assuming input is non-null array of 16 doubles, copies to output.
* \param input not-NULL array (only checked with assert() ) of 16 doubles, row-wise, meaning row 0, left to right, then row 1, left to right, etc.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CopyMatrix(double *input, igtl::Matrix4x4& output);

/**
* \brief Returns the difference in nanoseconds, as a positive number or zero.
* \param timeA must be not-NULL, only checked with assert()
* \param timeB must be not-NULL, only checked with assert()
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT igtlUint64 GetDifferenceInNanoSeconds(igtl::TimeStamp* timeA, igtl::TimeStamp* timeB);

/**
* \brief Returns true if the message is a STATUS_OK and false otherwise.
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsKeepAlive(const igtl::MessageBase::Pointer&);

/**
* \brief Returns true if the message is an igtl::StringMessage containing just the text "STATS".
*/
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool IsStatsRequest(const igtl::MessageBase::Pointer&);

} // end namespace niftk

#endif // NiftyLinkUtils_h
