/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestTrackingClient_h
#define TestTrackingClient_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpClient.h>

#include <igtlTimeStamp.h>

#include <QObject>
#include <QTimer>

namespace niftk
{

class TestTrackingClient : public QObject
{

  Q_OBJECT

public:
  TestTrackingClient(const std::string& hostName, const int& portNumber, const int& fps, const int& numberMessages, QObject *parent=0);
  virtual ~TestTrackingClient();

private slots:

  void Start();
  void OnFailedToSendKeepAliveMessage();
  void OnNoIncommingData();
  void OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg);
  void OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

  void OnBytesSent(qint64 bytes);
  void OnSocketError(int portNumber, QAbstractSocket::SocketError socketError);
  void OnConnectedToServer();
  void OnTimeOut();

private:
  QString             m_HostName;
  int                 m_PortNumber;
  int                 m_IntendedNumberMessages;
  NiftyLinkTcpClient *m_Client;

  QTimer             *m_Timer;
  int                 m_NumberMessagesSent;
};

} // end namespace niftk

#endif // TestTrackingClient_h
