/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef __NiftyLinkImageMessage_h
#define __NiftyLinkImageMessage_h

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "NiftyLinkMessage.h"
#include "NiftyLinkUtils.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

/**
* \class NiftyLinkImageMessage
* \brief Message type to hold image data.
*
* NiftyLinkImageMessage hold data related with the OpenIGTLink "IMAGE" message type.
* It offers functions to set and get the member variables and to intilize the
* message with random or known test data. It also includes a bunch of static member
* functions to create the related GET_ STT_ STP_ RTS_ messages which are used to
* control the data flow.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkImageMessage : public NiftyLinkMessage
{

public:

  typedef NiftyLinkImageMessage Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which sets the message and data types to "IMAGE"
  NiftyLinkImageMessage(void);

  /// \brief Basic destructor
  ~NiftyLinkImageMessage(void);

  /// \brief Basic copy constructor required for this data type
  NiftyLinkImageMessage(const NiftyLinkImageMessage &other);

  /// \brief Function which creates a GET_IMAGE message (igtl::GetImageMessage)
  static void Create_GET(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STT_IMAGE message (igtl::STTImageMessage)
  static void Create_STT(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a STP_IMAGE message (igtl::STPImageMessage)
  static void Create_STP(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Function which creates a RTS_IMAGE message (igtl::RTSImageMessage)
  static void Create_RTS(NiftyLinkMessage::Pointer &msgToCreate);

  /// \brief Set the image dimensions
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
  /// This is done automatically by SetQImage()
  void SetDimensions(int s[3]);

  /// \brief Set the image dimensions
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
  /// This is done automatically by SetQImage()
  void SetDimensions(int i, int j, int k);

  /// \brief Get the image dimensions
  void GetDimensions(int s[3]);

  /// \brief Get the image dimensions
  void GetDimensions(int &i, int &j, int &k);

  /// \brief Set the image sub volume
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
  int SetSubVolume(int dim[3], int off[3]);

  /// \brief Set the image sub volume
  /// SetDimensions() should be called prior to SetSubVolume(), since SetDimensions()
  /// sets subvolume parameters automatically assuming that subvolume = entire volume.
  int SetSubVolume(int dimi, int dimj, int dimk, int offi, int offj, int offk);

  /// \brief Get the image sub volume
  void GetSubVolume(int dim[3], int off[3]);

  /// \brief Get the image sub volume
  void GetSubVolume(int &dimi, int &dimj, int &dimk, int &offi, int &offj, int &offk);

  /// \brief Set the number of image components.
  /// This is done automatically by SetQImage()
  void SetNumComponents(int num);

  /// \brief Get the number of image components.
  int GetNumComponents();

  /// \brief Sets the image's origin / orientation matrix
  void SetMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Returns the image's origin / orientation matrix
  void GetMatrix(igtl::Matrix4x4 &matrix);

  /// \brief Set the matrixset flag to true to prevent the image matrix being
  /// destroyed during pack and unpack. This should be done when messages are received.
  void PreserveMatrix();

  /// \brief Sets the image's normals
  void SetNormals(float t[3], float s[3], float n[3]);

  /// \brief Returns the image's normals
  void GetNormals(float t[3], float s[3], float n[3]);

  // Spacings
  /// \brief Sets the image's spacing in a float array format
  void SetSpacing(float s[3]);

  /// \brief Sets the image's spacing
  void SetSpacing(float si, float sj, float sk);

  /// \brief Returns the image's normals in a float array
  void GetSpacing(float s[3]);

  /// \brief Returns the image's normals
  void GetSpacing(float &si, float &sj, float &sk);

  // Origin coordinate
  /// \brief Sets the image's origin in a float array format
  void SetOrigin(float p[3]);

  /// \brief Sets the image's origin
  void SetOrigin(float px, float py, float pz);

  /// \brief Returns the image's origin in a float array format
  void GetOrigin(float p[3]);

  /// \brief Returns the image's origin
  void GetOrigin(float &px, float &py, float &pz);

  /// \brief Loads an image from file and wraps it into the message
  void Load(QString filename);

  /// \brief Saves the message's contents
  void Save(QString filename);

  /// \brief Sets the message's contents in QImage format
  void SetQImage(const QImage& image);

  /// \brief Returns the message's contents in QImage format
  QImage GetQImage(void);

  /// \brief Initializes the message with known test data
  virtual void InitializeWithTestData(void);

  /// \brief Initializes the message with random test data
  virtual void InitializeWithRandomData(void);

protected:

private:

};

#endif
