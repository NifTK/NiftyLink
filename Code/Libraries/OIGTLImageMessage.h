/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

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
 * OIGTLImageMessage hold data related with the OpenIGTLink "IMAGE" message type.
 * It offers functions to set and get the member variables and to intilize the
 * message with random or known test data. It also includes a bunch of static member
 * functions to create the related GET_ STT_ STP_ RTS_ messages which are used to
 * control the data flow.
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

	/// \brief Set the image dimensions
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
	/// This is done automatically by setQimage()
  void SetDimensions(int s[3]);

	/// \brief Set the image dimensions
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
	/// This is done automatically by setQimage()
  void SetDimensions(int i, int j, int k);

	/// \brief Get the image dimensions
  void GetDimensions(int s[3]);

	/// \brief Get the image dimensions
  void GetDimensions(int &i, int &j, int &k);

	/// \brief Set the image sub volume
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
  int  SetSubVolume(int dim[3], int off[3]);

	/// \brief Set the image sub volume
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
  int  SetSubVolume(int dimi, int dimj, int dimk, int offi, int offj, int offk);

	/// \brief Get the image sub volume
  void GetSubVolume(int dim[3], int off[3]);

	/// \brief Get the image sub volume
  void GetSubVolume(int &dimi, int &dimj, int &dimk, int &offi, int &offj, int &offk);

  /// \brief Set the number of image components.
	/// This is done automatically by setQimage()
	void SetNumComponents(int num);

  /// \brief Get the number of image components.
  int  GetNumComponents();
  
  /// \brief Sets the image's origin / orientation matrix
  void setMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Returns the image's origin / orientation matrix
	void getMatrix(igtl::Matrix4x4 &matrix);

	/// \brief Set the matrixset flag to true to prevent the image matrix being 
	/// destroyed during pack and unpack. This should be done when messages are received.
	void PreserveMatrix();

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
  void setQImage(const QImage& image);

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
