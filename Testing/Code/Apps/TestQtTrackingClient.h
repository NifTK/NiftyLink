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

class TestQtTrackingClient : public QObject
{

  Q_OBJECT

public:
  TestQtTrackingClient(const std::string& hostName, const int& portNumber, const int& fps, const int& numberMessages, QObject *parent=0);
  virtual ~TestQtTrackingClient();

private slots:

  void Start();
  void OnTimeOut();
  void OnConnectedToServer();
  void OnBytesSent(qint64 bytes);

private:
  QString             m_HostName;
  int                 m_PortNumber;
  int                 m_IntendedNumberMessages;
  NiftyLinkTcpClient *m_Client;
  QTimer             *m_Timer;
  int                 m_NumberMessagesSent;
};

} // end namespace niftk

#endif // TestQtTrackingClient_h
