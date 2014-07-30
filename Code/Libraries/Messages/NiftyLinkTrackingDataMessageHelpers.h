/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTrackingDataMessageHelpers_h
#define NiftyLinkTrackingDataMessageHelpers_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlTrackingDataMessage.h>

#include <QString>

/**
 * \file NiftyLinkTrackingDataMessageHelpers.h
 * \brief Helper methods to initialise TDATA messages.
 */
namespace niftk
{

/// \brief Initialises the tracking data with the test matrix, applying the same matrix to all elements in the message.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTrackingDataWithTestData(const igtl::Matrix4x4& testMatrix, igtl::TrackingDataMessage::Pointer& messageToWriteTo);

/// \brief Initialises the tracking data with a random matrix.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTrackingDataWithRandomData(igtl::TrackingDataMessage::Pointer& messageToWriteTo);

/// \brief Retrieves a simple string representation of the matrix from the message, where the
/// index is the element number. No validation is performed on the index number.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetMatrixAsString(const igtl::TrackingDataMessage::Pointer& message, int elementIndex=0);

/// \brief Creates a tracking data message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName, const QString& hostName, int portNumber, igtl::Matrix4x4& input);

/// \brief Creates a tracking data message within a NiftyLinkMessageContainer, that is packed and ready to go.
/// \param input should be an array of 16 doubles assumed to be in row-order.
/// <pre>
/// input[0][0] = input[0];
/// input[0][1] = input[1];
/// input[0][2] = input[2];
/// input[0][3] = input[3];
/// input[1][0] = input[4];
/// input[3][0] = input[12];
/// etc.
/// </pre>
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName, const QString& hostName, int portNumber, double* input);

} // end namespace

#endif // NiftyLinkTrackingDataMessageHelpers_h
