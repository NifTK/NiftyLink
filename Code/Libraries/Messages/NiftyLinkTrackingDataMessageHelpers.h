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

#include <igtlTrackingDataMessage.h>

#include <QString>

/**
 * \file Helper methods to initialise TDATA messages.
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

} // end namespace

#endif // NiftyLinkTrackingDataMessageHelpers_h
