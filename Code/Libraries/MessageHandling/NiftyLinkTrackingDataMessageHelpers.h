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
/**
* \name NiftyLinkTrackingDataMessageHelpersForTesting
* \brief For testing purposes, helper methods to initialise TDATA messages.
*/
///@{

/// \brief Initialises the tracking data with the test matrix, applying the same matrix to all elements in the message.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTrackingDataWithTestData(const igtl::Matrix4x4& testMatrix,
                                                                               igtl::TrackingDataMessage::Pointer& messageToWriteTo);

/// \brief Initialises the tracking data with a random matrix, calling InitialiseTrackingDataWithTestData.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTrackingDataWithRandomData(igtl::TrackingDataMessage::Pointer& messageToWriteTo);

/// \brief Creates a test message, with a single matrix of random data.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessageWithRandomData();

/// \brief Creates a test message, containing a configurable number of tracking matrix elements, each with random tracking data.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessageWithRandomData(
    igtl::TimeStamp::Pointer& timeStamp,
    const int& matricesPerMessage);

///@}

/**
* \name NiftyLinkTrackingDataMessageHelpers
* \brief Helper methods to initialise TDATA messages.
*/
///@{

/// \brief Retrieves a simple string representation of the matrix from the message, where the
/// index is the element number. No validation is performed on the index number.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetMatrixAsString(const igtl::TrackingDataMessage::Pointer& message, int elementIndex=0);

/// \breif Retrieves all matrices from the message.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetMatricesAsString(const igtl::TrackingDataMessage::Pointer& message);

/// \brief Creates a single matrix tracking data message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName,
                                                                                                    const QString& toolName,
                                                                                                    const QString& hostName,
                                                                                                    const int& portNumber,
                                                                                                    const igtl::Matrix4x4& matrix,
                                                                                                    igtl::TimeStamp::Pointer& timeCreated
                                                                                                    );

/// \brief Creates a single matrix tracking data message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName,
                                                                                                    const QString& toolName,
                                                                                                    const QString& hostName,
                                                                                                    const int& portNumber,
                                                                                                    const igtl::Matrix4x4& matrix
                                                                                                    );

/// \brief Creates a single matrix tracking data message within a NiftyLinkMessageContainer, that
///        is packed and ready to go.
/// \param input should be an array of 16 doubles assumed to be in row-order.
/// \code{.cpp}
/// input[0][0] = input[0];
/// input[0][1] = input[1];
/// input[0][2] = input[2];
/// input[0][3] = input[3];
/// input[1][0] = input[4];
/// input[3][0] = input[12];
/// etc.
/// \endcode
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTrackingDataMessage(const QString& deviceName,
                                                                                                    const QString& toolName,
                                                                                                    const QString& hostName,
                                                                                                    const int& portNumber,
                                                                                                    const double* input);
///@}

} // end namespace

#endif // NiftyLinkTrackingDataMessageHelpers_h
