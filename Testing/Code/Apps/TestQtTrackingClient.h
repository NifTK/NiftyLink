/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestQtTrackingClient_h
#define TestQtTrackingClient_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpClient.h>

#include <igtlTimeStamp.h>

#include <QObject>
#include <QTimer>

namespace niftk
{

/**
 * \class TestQtTrackingClient
 * \brief Qt based tracking client to send/receive data from an OpenIGTLink server.
 */
class TestQtTrackingClient : public QObject
{

  Q_OBJECT

public:
  TestQtTrackingClient(const std::string& hostName,
                       const int& portNumber,
                       const int& fps,
                       const int& totalNumberMessages,
                       const int& trackedObjectsPerMessage,
                       QObject *parent=0
                      );
  virtual ~TestQtTrackingClient();

private slots:

  void Start();
  void OnConnectedToServer();

private:

  void RunTest();
  NiftyLinkMessageContainer::Pointer CreateTrackingData();

  QString             m_HostName;
  int                 m_PortNumber;
  int                 m_FramesPerSecond;
  int                 m_IntendedNumberMessages;
  int                 m_TrackedObjectsPerMessage;
  int                 m_NumberMessagesSent;
  NiftyLinkTcpClient *m_Client;

};

} // end namespace niftk

#endif // TestQtTrackingClient_h
