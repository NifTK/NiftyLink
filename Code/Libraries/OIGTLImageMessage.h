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

#ifndef OIGTLIMAGEMESSAGE_H
#define OIGTLIMAGEMESSAGE_H

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "OIGTLMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
 * \class OIGTLImageMessage
 * \brief Message type to hold image data.
 * 
 * OIGTLImageMessage hold data related with the OpenIGTLink "IMAGE" message type. It offers functions to set and get the member variables and to intilize the message with random or known test data.
 * It also includes a bunch of static member functions to create the related GET_ STT_ STP_ RTS_ messages which are used to control the data flow. 
 */

class NIFTYLINKCOMMON_WINEXPORT OIGTLImageMessage : public OIGTLMessage
{

public:

	typedef OIGTLImageMessage		                      Self;
	typedef QExplicitlySharedDataPointer<Self>        Pointer;
	typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;

  /// \brief Basic constructor which sets the message and data types to "IMAGE"
	OIGTLImageMessage(void);
  /// \brief Basic destructor
	~OIGTLImageMessage(void);
  /// \brief Basic copy constructor required for this data type
	OIGTLImageMessage(const OIGTLImageMessage &other); 

  /// \brief Function which creates a GET_IMAGE message (igtl::GetImageMessage)
	static void Create_GET(OIGTLMessage::Pointer &msgToCreate);
  /// \brief Function which creates a STT_IMAGE message (igtl::STTImageMessage)
	static void Create_STT(OIGTLMessage::Pointer &msgToCreate);
  /// \brief Function which creates a STP_IMAGE message (igtl::STPImageMessage)
	static void Create_STP(OIGTLMessage::Pointer &msgToCreate);
  /// \brief Function which creates a RTS_IMAGE message (igtl::RTSImageMessage)
	static void Create_RTS(OIGTLMessage::Pointer &msgToCreate);

	//    TODO: Add these functions
  /*
    // Image dimensions.
  // SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  // sets subvolume parameters automatically assuming that subvolume = entire volume.
  void SetDimensions(int s[3]);
  void SetDimensions(int i, int j, int k);
  void GetDimensions(int s[3]);
  void GetDimensions(int &i, int &j, int &k);

  // Sub-volume dimensions and offset.
  // SetSubVolume() should be called after calling SetDiemensions(), since SetDimensions()
  // reset the subvolume parameters automatically.
  int  SetSubVolume(int dim[3], int off[3]);
  int  SetSubVolume(int dimi, int dimj, int dimk, int offi, int offj, int offk);
  void GetSubVolume(int dim[3], int off[3]);
  void GetSubVolume(int &dimi, int &dimj, int &dimk, int &offi, int &offj, int &offk);

    // Number of components
  void SetNumComponents(int num);
  int  GetNumComponents();
  */
  
  /// \brief Sets the image's origin / orientation matrix
  void setMatrix(igtl::Matrix4x4 &matrix);
  /// \brief Returns the image's origin / orientation matrix
	void getMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Sets the image's normals
	void setNormals(float t[3], float s[3], float n[3]);
  /// \brief Returns the image's normals
	void getNormals(float t[3], float s[3], float n[3]);

  // Spacings
  /// \brief Sets the image's spacing in a float array format
  void setSpacing(float s[3]);
  /// \brief Sets the image's spacing
  void setSpacing(float si, float sj, float sk);
  /// \brief Returns the image's normals in a float array
  void getSpacing(float s[3]);
  /// \brief Returns the image's normals 
  void getSpacing(float &si, float &sj, float &sk);

  // Origin coordinate
  /// \brief Sets the image's origin in a float array format
  void setOrigin(float p[3]);
  /// \brief Sets the image's origin
  void setOrigin(float px, float py, float pz);
  /// \brief Returns the image's origin in a float array format
  void getOrigin(float p[3]);
  /// \brief Returns the image's origin
  void getOrigin(float &px, float &py, float &pz);

  /// \brief Loads an image from file and wraps it into the message
  void load(QString filename);
  /// \brief Saves the message's contents
  void save(QString filename);

  /// \brief Sets the message's contents in QImage format
  void setQImage(QImage image);
  /// \brief Returns the message's contents in QImage format
  QImage getQImage(void);

  /// \brief Initializes the message with known test data
	virtual void initializeWithTestData(void);
  /// \brief Initializes the message with random test data
	virtual void initializeWithRandomData(void);

protected:

private:

};

#endif
