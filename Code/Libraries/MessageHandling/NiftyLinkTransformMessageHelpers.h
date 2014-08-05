/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTransformMessageHelpers_h
#define NiftyLinkTransformMessageHelpers_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlTransformMessage.h>

#include <QString>

/**
 * \file NiftyLinkTransformMessageHelpers.h
 * \brief Helper methods to initialise TRANSFORM messages.
 */
namespace niftk
{

/**
 * \name NiftyLinkTransformMessageHelpers
 * \brief Helper methods to initialise TRANSFORM messages.
 */
///@{

/// \brief Initialises the transform message with the test matrix, applying the same matrix to all elements.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTransformWithTestData(const igtl::Matrix4x4& testMatrix, igtl::TransformMessage::Pointer& messageToWriteTo);

/// \brief Initialises the transform message with a random matrix.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTransformWithRandomData(igtl::TransformMessage::Pointer& messageToWriteTo);

/// \brief Retrieves a simple string representation of the matrix from the message.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetMatrixAsString(const igtl::TransformMessage::Pointer& message);

/// \brief Creates a transform message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTransformMessage(const QString& deviceName, const QString& hostName, int portNumber, igtl::Matrix4x4& input);

/// \brief Creates a transform message within a NiftyLinkMessageContainer, that is packed and ready to go.
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
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateTransformMessage(const QString& deviceName, const QString& hostName, int portNumber, double* input);

///@}

} // end namespace

#endif // NiftyLinkTransformMessageHelpers_h
