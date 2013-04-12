/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef __NiftyLinkStatusMessage_h
#define __NiftyLinkStatusMessage_h

#include "NiftyLinkMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
* \class NiftyLinkStatusMessage
* \brief Message type to hold status info.
*
* NiftyLinkStatusMessage hold data related with the OpenIGTLink "STATUS" message type.
* It offers functions to set and get the member variables and to intilize the message
* with random or known test data. It also includes a bunch of static member functions
* to create the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkStatusMessage : public NiftyLinkMessage
{

public:

  typedef NiftyLinkStatusMessage Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which sets the message and data types to "STATUS"
  NiftyLinkStatusMessage(void);

  /// \brief Basic destructor
  virtual ~NiftyLinkStatusMessage(void);

  /// \brief Basic copy constructor required for this data type
  NiftyLinkStatusMessage(const NiftyLinkStatusMessage &other);

  /// \brief Function which creates a GET_STATUS message (igtl::GetStatusMessage)
  static void Create_GET(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_STATUS message (igtl::STTStatusMessage)
  static void Create_STT(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_STATUS message (igtl::STPStatusMessage)
  static void Create_STP(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_STATUS message (igtl::RTSStatusMessage)
  static void Create_RTS(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Sets the error code
  void SetErrorCode(int code);

  /// \brief Returns the current error code
  int GetErrorCode();

  /// \brief Sets the error sub-code
  void SetErrorSubCode(igtlInt64 subcode);

  /// \brief Returns the current error sub-code
  igtlInt64 GetErrorSubCode();

  /// \brief Sets error string
  void SetErrorName(QString name);

  /// \brief Returns the current error string
  QString GetErrorName();

  /// \brief Sets the status string
  void SetStatusString(QString str);

  /// \brief Returns the current status string
  QString GetStatusString();

  /// \brief Initializes the message with known test data
  virtual void InitializeWithTestData(void);

private:

};

#endif
