/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkStringMessage_h
#define NiftyLinkStringMessage_h

#include "NiftyLinkMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

namespace niftk
{

/**
* \class NiftyLinkStringMessage
* \brief Message type to hold status info.
*
* NiftyLinkStringMessage hold data related with the OpenIGTLink "STRING" message type.
* It offers functions to set and get the member variables and to intilize the message
* with random or known test data. It also includes a bunch of static member functions
* to create the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkStringMessage : public NiftyLinkMessage
{

public:
  typedef NiftyLinkStringMessage Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which sets the message and data types to "STRING"
  NiftyLinkStringMessage(void);

  /// \brief Basic destructor
  virtual ~NiftyLinkStringMessage(void);

  /// \brief Basic copy constructor required for this data type
  NiftyLinkStringMessage(const NiftyLinkStringMessage &other);

  /// \brief Sets the message string
  void SetString(QString string);

  /// \brief Sets the encoding
  void SetEncoding(igtlUint16 enc);

  /// \brief Returns the current message string
  QString GetString(void);

  /// \brief Returns the current message encoding
  igtlUint16 GetEncoding(void);

  /// \brief Initializes the message with known test data
  virtual void InitializeWithTestData(void);

private:

};

} // end namespace niftk

#endif // NiftyLinkStringMessage_h
