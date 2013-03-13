/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef __NiftyLinkTrackingDataMessage_h
#define __NiftyLinkTrackingDataMessage_h

#include "NiftyLinkMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
* \class NiftyLinkTrackingDataMessage
* \brief Message type to hold transform data.
*
* NiftyLinkTrackingDataMessage hold data related with the OpenIGTLink "TRACKINGDATA" message type.
* It offers functions to set and get the member variables and to intilize the message with
* random or known test data. It also includes a bunch of static member functions to create
* the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.
*/

class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTrackingDataMessage : public NiftyLinkMessage
{

public:

  /// Tracking data type
  enum
  {
    TYPE_TRACKER = 1, /* Tracker */
    TYPE_6D = 2, /* 6D instrument (regular instrument) */
    TYPE_3D = 3, /* 3D instrument (only tip of the instrument defined) */
    TYPE_5D = 4, /* 5D instrument (tip and handle are defined, but not the normal vector) */
  };


  typedef NiftyLinkTrackingDataMessage Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which sets the message and data types to "TRACKINGDATA"
  NiftyLinkTrackingDataMessage(void);

  /// \brief Basic destructor
  ~NiftyLinkTrackingDataMessage(void);

  /// \brief Basic copy constructor required for this data type
  NiftyLinkTrackingDataMessage(const NiftyLinkTrackingDataMessage &other);

  /// \brief Function which creates a GET_TRACKINGDATA message (igtl::GetTrackingDataMessage)
  static void Create_GET(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_TRACKINGDATA message (igtl::STTTrackingDataMessage)
  static void Create_STT(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_TRACKINGDATA message (igtl::STPTrackingDataMessage)
  static void Create_STP(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_TRACKINGDATA message (igtl::RTSTrackingDataMessage)
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

  /// \brief Sets the tracker tool's name
  void SetTrackerToolName(QString name);

  /// \brief Gets the tracker tool's name
  QString GetTrackerToolName();

  /// \brief Sets the tracker tool's type
  void SetTrackerToolType(igtlUint8 type);

  /// \brief Gets the tracker tool's name
  igtlUint8 GetTrackerToolType();

  /// \brief Initializes the message with known test data
  virtual void InitializeWithTestData(igtl::Matrix4x4& testMatrix);

  /// \brief Initializes the message with random test data
  virtual void InitializeWithRandomData(void);

private:
  float m_p[3];
};

#endif
