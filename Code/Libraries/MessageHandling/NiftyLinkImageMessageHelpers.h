/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkImageMessageHelpers_h
#define NiftyLinkImageMessageHelpers_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlImageMessage.h>

#include <QImage>

/**
 * \file NiftyLinkImageMessageHelpers.h
 * \brief Helper methods to load/save images, initialise with random test data, and set/get from QImage.
 */
namespace niftk
{

/**
 * \name NiftyLinkImageMessageHelpers
 * \brief Helper methods to load/save images, initialise with random test data, and set/get from QImage.
 */
///@{

/// \brief Loads a standard image from the Qt resource system, and writes it to the provided image.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseImageWithTestData(igtl::ImageMessage::Pointer& image);

/// \brief Loads a standard image, and puts a randomly positioned rectangle in it, and writes it to the provided image message.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void InitialiseImageWithRandomData(igtl::ImageMessage::Pointer& image);

/// \brief Sets the QImage onto the igtl::ImageMessage.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void SetQImage(const QImage& imageToRead, igtl::ImageMessage::Pointer& imageToWrite);

/// \brief Gets/Creates a new QImage using the provided igtl::ImageMessage.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void GetQImage(const igtl::ImageMessage::Pointer& imageToRead, QImage& imageToWrite);

/// \brief Saves the image data to a file.
extern "C++" NIFTYLINKCOMMON_WINEXPORT void SaveImage(const igtl::ImageMessage::Pointer& imageToRead, const QString& outputFileName);

/// \brief Loads the image data from file, and writes it into the igtl::ImageMessage;
extern "C++" NIFTYLINKCOMMON_WINEXPORT void LoadImage(const QString& fileName, igtl::ImageMessage::Pointer& imageToWrite);

/// \brief Creates an image message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateImageMessage(const QString& deviceName, const QString& hostName, int portNumber, const QImage& imageToRead);

///@}

} // end namesspace niftk

#endif // NiftyLinkImageMessageHelpers_h
