/*=============================================================================

 NiftyLink:  A software library to facilitate communication over OpenIGTLink.

             http://cmic.cs.ucl.ac.uk/
             http://www.ucl.ac.uk/

 Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

 Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
 Revision          : $Revision: 3300 $
 Last modified by  : $Author: mjc $

 Original author   : m.clarkson@ucl.ac.uk

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 ============================================================================*/

#ifndef OIGTLSTATUSMESSAGE_H
#define OIGTLSTATUSMESSAGE_H

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
 * \class OIGTLStatusMessage
 * \brief Message type to hold status info.
 * 
 * OIGTLStatusMessage hold data related with the OpenIGTLink "STATUS" message type.
 * It offers functions to set and get the member variables and to intilize the message
 * with random or known test data. It also includes a bunch of static member functions
 * to create the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.
 */
class NIFTYLINKCOMMON_WINEXPORT OIGTLStatusMessage : public OIGTLMessage
{

public:

  typedef OIGTLStatusMessage		                    Self;
  typedef QExplicitlySharedDataPointer<Self>        Pointer;
  typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;
  
  /// \brief Basic constructor which sets the message and data types to "STATUS"
  OIGTLStatusMessage(void);

  /// \brief Basic destructor
  ~OIGTLStatusMessage(void);

  /// \brief Basic copy constructor required for this data type
  OIGTLStatusMessage(const OIGTLStatusMessage &other); 

  /// \brief Function which creates a GET_STATUS message (igtl::GetStatusMessage)
  static void Create_GET(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_STATUS message (igtl::STTStatusMessage)
  static void Create_STT(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_STATUS message (igtl::STPStatusMessage)
  static void Create_STP(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_STATUS message (igtl::RTSStatusMessage)
  static void Create_RTS(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Sets the error code
  void setErrorCode(int code);

  /// \brief Returns the current error code
  int  getErrorCode();

  /// \brief Sets the error sub-code
  void setErrorSubCode(igtlInt64 subcode);

  /// \brief Returns the current error sub-code
  igtlInt64 getErrorSubCode();
  
  /// \brief Sets error string
  void    setErrorName(QString name);

  /// \brief Returns the current error string
  QString getErrorName();

  /// \brief Sets the status string
  void    setStatusString(QString str);

  /// \brief Returns the current status string
  QString getStatusString();

  /// \brief Initializes the message with known test data
  virtual void initializeWithTestData(void);

private:

};

#endif
