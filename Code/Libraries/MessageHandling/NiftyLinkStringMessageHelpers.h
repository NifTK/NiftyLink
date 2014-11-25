/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkStringMessageHelpers_h
#define NiftyLinkStringMessageHelpers_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlStringMessage.h>

#include <QString>

/**
* \file NiftyLinkStringMessageHelpers.h
* \brief Helper methods to initialise STRING messages.
*/
namespace niftk
{

///
/**
* \name NiftyLinkStringMessageHelpers
* \brief Helper methods to initialise STRING messages.
*/
///@{

/// \brief Creates a string message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateStringMessage(const QString& deviceName,
                                                                                              const QString& hostName,
                                                                                              const int& portNumber,
                                                                                              const QString& stringMessage,
                                                                                              igtl::TimeStamp::Pointer& timeCreated
                                                                                              );

/// \brief Creates a string message within a NiftyLinkMessageContainer, that is packed and ready to go.
extern "C++" NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer::Pointer CreateStringMessage(const QString& deviceName,
                                                                                              const QString& hostName,
                                                                                              const int& portNumber,
                                                                                              const QString& stringMessage
                                                                                              );
///@}

} // end namespace

#endif // NiftyLinkStringMessageHelpers_h
