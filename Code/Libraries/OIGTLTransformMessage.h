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

#ifndef OIGTLTRANSFORMMESSAGE_H
#define OIGTLTRANSFORMMESSAGE_H

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
 * \class OIGTLTransformMessage
 * \brief Message type to hold transform data.
 * 
 * OIGTLTransformMessage hold data related with the OpenIGTLink "TRANSFORM" message type.
 * It offers functions to set and get the member variables and to intilize the message with
 * random or known test data. It also includes a bunch of static member functions to create
 * the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow.
 */
class NIFTYLINKCOMMON_WINEXPORT OIGTLTransformMessage : public OIGTLMessage
{

public:

  typedef OIGTLTransformMessage		                  Self;
  typedef QExplicitlySharedDataPointer<Self>        Pointer;
  typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;
  
  /// \brief Basic constructor which sets the message and data types to "TRANSFORM"
  OIGTLTransformMessage(void);

  /// \brief Basic destructor
  ~OIGTLTransformMessage(void);

  /// \brief Basic copy constructor required for this data type
  OIGTLTransformMessage(const OIGTLTransformMessage &other); 

  /// \brief Function which creates a GET_TRANSFORM message (igtl::GetTransformMessage)
  static void Create_GET(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_TRANSFORM message (igtl::STTTransformMessage)
  static void Create_STT(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_TRANSFORM message (igtl::STPTransformMessage)
  static void Create_STP(OIGTLMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_TRANSFORM message (igtl::RTSTransformMessage)
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

  /// \brief Sets the normals' array
  void setNormals(float o[3][3]);

  /// \brief Gets the normals' array
  void getNormals(float o[3][3]);

  /// \brief Sets the normals' components
  void setNormals(float t[3], float s[3], float n[3]);

  /// \brief Gets the normals' components
  void getNormals(float t[3], float s[3], float n[3]);

  /// \brief Initializes the message with known test data
  virtual void initializeWithTestData(igtl::Matrix4x4& testMatrix);

  /// \brief Initializes the message with random test data
  virtual void initializeWithRandomData(void);

private:
  float m_p[3];
};

#endif
