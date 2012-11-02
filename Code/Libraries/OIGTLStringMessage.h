/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef OIGTLSTRINGMESSAGE_H
#define OIGTLSTRINGMESSAGE_H

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
 * \class OIGTLStringMessage
 * \brief Message type to hold status info.
 * 
 * OIGTLStringMessage hold data related with the OpenIGTLink "STRING" message type.
 * It offers functions to set and get the member variables and to intilize the message
 * with random or known test data. It also includes a bunch of static member functions
 * to create the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.
 */
class NIFTYLINKCOMMON_WINEXPORT OIGTLStringMessage : public OIGTLMessage
{

public:

  typedef OIGTLStringMessage		                    Self;
  typedef QExplicitlySharedDataPointer<Self>        Pointer;
  typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;
  
  /// \brief Basic constructor which sets the message and data types to "STRING"
  OIGTLStringMessage(void);

  /// \brief Basic destructor
  ~OIGTLStringMessage(void);

  /// \brief Basic copy constructor required for this data type
  OIGTLStringMessage(const OIGTLStringMessage &other); 

  /// \brief Function which creates a GET_STRING message (igtl::GetStringMessage)
  static void Create_GET(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_STRING message (igtl::STTStringMessage)
  static void Create_STT(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_STRING message (igtl::STPStringMessage)
  static void Create_STP(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_STRING message (igtl::RTSStringMessage)
  static void Create_RTS(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Sets the message string
  void setString(QString string);

  /// \brief Sets the encoding
  void setEncoding(igtlUint16 enc);

  /// \brief Returns the current message string
  QString getString(void);

  /// \brief Returns the current message encoding
  igtlUint16 getEncoding(void);

  /// \brief Initializes the message with known test data
  virtual void initializeWithTestData(void);

private:

};

#endif
