/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestServer_h
#define TestServer_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpServer.h>

#include <igtlTimeStamp.h>

#include <QObject>

namespace niftk
{

class TestServer : public QObject
{

  Q_OBJECT

public:
  TestServer(const int& portNumber, const bool& isEchoing, QObject *parent=0);
  virtual ~TestServer();

private slots:

  void Start();
  void OnSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  void OnFailedToSendKeepAliveMessage();
  void OnNoIncommingData();

  void OnClientConnected(int portNumber);
  void OnMessageReceived(int portNumber, niftk::NiftyLinkMessageContainer::Pointer msg);
  void OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

private:
  int             m_PortNumber;
  bool            m_IsEchoing;
  NiftyLinkTcpServer *m_Server;
};

} // end namespace niftk

#endif // TestServer_h
