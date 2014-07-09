/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkTransformMessage_h
#define NiftyLinkTransformMessage_h

#include "NiftyLinkCommonWin32ExportHeader.h"

#include "NiftyLinkMessage.h"
#include <NiftyLinkUtils.h>

/**
* \class NiftyLinkTransformMessage
* \brief Message type to hold transform data.
*
* NiftyLinkTransformMessage hold data related with the OpenIGTLink "TRANSFORM" message type.
* It offers functions to set and get the member variables and to intilize the message with
* random or known test data. It also includes a bunch of static member functions to create
* the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.*/

class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTransformMessage : public NiftyLinkMessage
{

public:

  typedef NiftyLinkTransformMessage Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which sets the message and data types to "TRANSFORM"
  NiftyLinkTransformMessage(void);

  /// \brief Basic destructor
  virtual ~NiftyLinkTransformMessage(void);

  /// \brief Basic copy constructor required for this data type
  NiftyLinkTransformMessage(const NiftyLinkTransformMessage &other);

  /// \brief Function which creates a GET_TRANSFORM message (igtl::GetTransformMessage)
  static void Create_GET(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_TRANSFORM message (igtl::STTTransformMessage)
  static void Create_STT(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_TRANSFORM message (igtl::STPTransformMessage)
  static void Create_STP(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_TRANSFORM message (igtl::RTSTransformMessage)
  static void Create_RTS(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Sets the transformation matrix
  void SetMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Gets the transformation matrix
  void GetMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Gets the transformation matrix as QString (for printing)
  QString GetMatrixAsString(void);

  /// \brief Sets the position array
  void SetPosition(float p[3]);

  /// \brief Gets the position array
  void GetPosition(float p[3]);

  /// \brief Sets the position coordinates
  void SetPosition(float px, float py, float pz);

  /// \brief Gets the position coordinates
  void GetPosition(float &px, float &py, float &pz);

  /// \brief Sets the normals' array
  void SetNormals(float o[3][3]);

  /// \brief Gets the normals' array
  void GetNormals(float o[3][3]);

  /// \brief Sets the normals' components
  void SetNormals(float t[3], float s[3], float n[3]);

  /// \brief Gets the normals' components
  void GetNormals(float t[3], float s[3], float n[3]);

  /// \brief Initializes the message with known test data
  virtual void InitializeWithTestData(igtl::Matrix4x4& testMatrix);

  /// \brief Initializes the message with random test data
  virtual void InitializeWithRandomData(void);

private:
  float m_p[3];
};

#endif // NiftyLinkTransformMessage_h
