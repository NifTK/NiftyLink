/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestClientNifTKQtTrackingAndImaging_h
#define TestClientNifTKQtTrackingAndImaging_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpClient.h>

#include <igtlTimeStamp.h>
#include <igtlImageMessage.h>

#include <QObject>
#include <QTimer>
#include <QImage>

namespace niftk
{

/**
 * \class TestClientNifTKQtTrackingAndImaging
 * \file TestClientNifTKQtTrackingAndImaging.h
 * \brief Qt based tracking client to send/receive imaging data from an OpenIGTLink server.
 */
class TestClientNifTKQtTrackingAndImaging : public QObject
{

  Q_OBJECT

public:
  TestClientNifTKQtTrackingAndImaging(const std::string& hostName,
                       const int& portNumber,
                       const int& fps,
                       const int& channels,
                       const int& totalNumberMessages,
                       const QString imageFileName,
                       QObject *parent=0
                      );
  virtual ~TestClientNifTKQtTrackingAndImaging();

private slots:

  void Start();
  void Shutdown();
  void OnConnectedToServer();
  void OnDisconnected();

private:

  void RunTest();

  igtl::ImageMessage::Pointer  m_ImageMessage;
  QString                      m_HostName;
  int                          m_PortNumber;
  int                          m_FramesPerSecond;
  int                          m_Channels;
  int                          m_IntendedNumberMessages;
  int                          m_NumberMessagesSent;
  NiftyLinkTcpClient          *m_Client;

};

} // end namespace niftk

#endif // TestClientNifTKQtTrackingAndImaging_h
