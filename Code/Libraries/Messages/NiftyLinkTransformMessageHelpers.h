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

#include <igtlTransformMessage.h>

#include <QString>

namespace niftk
{

/// \brief Initialises the tracking data with the test matrix, applying the same matrix to all elements.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTransformWithTestData(const igtl::Matrix4x4& testMatrix, igtl::TransformMessage::Pointer& messageToWriteTo);

/// \brief Initialises the tracking data with a random matrix.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseTransformWithRandomData(igtl::TransformMessage::Pointer& messageToWriteTo);

/// \brief Retrieves a simple string representation of the matrix from the message.
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString GetMatrixAsString(const igtl::TransformMessage::Pointer& message);

} // end namespace

#endif // NiftyLinkTransformMessageHelpers_h
