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

#ifndef OIGTLTRACKINGDATAMESSAGE_H
#define OIGTLTRACKINGDATAMESSAGE_H

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
 * \class OIGTLTrackingDataMessage
 * \brief Message type to hold transform data.
 * 
 * OIGTLTrackingDataMessage hold data related with the OpenIGTLink "TRACKINGDATA" message type. It offers functions to set and get the member variables and to intilize the message with random or known test data.
 * It also includes a bunch of static member functions to create the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow. 
 */

class NIFTYLINKCOMMON_WINEXPORT OIGTLTrackingDataMessage : public OIGTLMessage
{

public:
  
  // Tracking data type
  enum {
    TYPE_TRACKER  = 1,  /* Tracker */
    TYPE_6D       = 2,  /* 6D instrument (regular instrument) */
    TYPE_3D       = 3,  /* 3D instrument (only tip of the instrument defined) */
    TYPE_5D       = 4,  /* 5D instrument (tip and handle are defined, but not the normal vector) */
  };


  typedef OIGTLTrackingDataMessage		              Self;
  typedef QExplicitlySharedDataPointer<Self>        Pointer;
  typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;
  
  /// \brief Basic constructor which sets the message and data types to "TRACKINGDATA"
  OIGTLTrackingDataMessage(void);
  /// \brief Basic destructor
  ~OIGTLTrackingDataMessage(void);
  /// \brief Basic copy constructor required for this data type
  OIGTLTrackingDataMessage(const OIGTLTrackingDataMessage &other); 

  /// \brief Function which creates a GET_TRACKINGDATA message (igtl::GetTrackingDataMessage)
  static void Create_GET(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_TRACKINGDATA message (igtl::STTTrackingDataMessage)
  static void Create_STT(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_TRACKINGDATA message (igtl::STPTrackingDataMessage)
  static void Create_STP(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_TRACKINGDATA message (igtl::RTSTrackingDataMessage)
  static void Create_RTS(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Sets the transformation matrix
  void setMatrix(igtl::Matrix4x4 &matrix);
  /// \brief Gets the transformation matrix
  void getMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Gets the transformation matrix as QString (for printing)
  QString getMatrixAsString(void);

  /// \brief Sets the position array
  void setPosition(float p[3]);
  /// \brief Gets the position array
  void getPosition(float p[3]);

  /// \brief Sets the position coordinates
  void setPosition(float px, float py, float pz);
  /// \brief Gets the position coordinates
  void getPosition(float &px, float &py, float &pz);

  /// \brief Sets the tracker tool's name
  void setTrackerToolName(QString name);
  /// \brief Gets the tracker tool's name
  QString getTrackerToolName();

  /// \brief Sets the tracker tool's type
  void setTrackerToolType(igtlUint8 type);
  /// \brief Gets the tracker tool's name
  igtlUint8 getTrackerToolType();

  /// \brief Initializes the message with known test data
  virtual void initializeWithTestData(void);
  /// \brief Initializes the message with random test data
  virtual void initializeWithRandomData(void);

private:
  float m_p[3];
};

#endif
