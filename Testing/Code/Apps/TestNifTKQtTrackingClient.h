/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestNifTKQtTrackingClient_h
#define TestNifTKQtTrackingClient_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpClient.h>

#include <igtlTimeStamp.h>

#include <QObject>
#include <QTimer>

namespace niftk
{

/**
 * \class TestNifTKQtTrackingClient
 * \brief Qt based tracking client to send/receive data from an OpenIGTLink server.
 */
class TestNifTKQtTrackingClient : public QObject
{

  Q_OBJECT

public:
  TestNifTKQtTrackingClient(const std::string& hostName,
                       const int& portNumber,
                       const int& fps,
                       const int& totalNumberMessages,
                       const int& trackedObjectsPerMessage,
                       QObject *parent=0
                      );
  virtual ~TestNifTKQtTrackingClient();

private slots:

  void Start();
  void OnConnectedToServer();

private:

  void RunTest();

  QString             m_HostName;
  int                 m_PortNumber;
  int                 m_FramesPerSecond;
  int                 m_IntendedNumberMessages;
  int                 m_TrackedObjectsPerMessage;
  int                 m_NumberMessagesSent;
  NiftyLinkTcpClient *m_Client;

};

} // end namespace niftk

#endif // TestNifTKQtTrackingClient_h
