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
#include <QTimer>

namespace niftk
{

class TestServer : public QObject
{

  Q_OBJECT

public:
  TestServer(const int& portNumber, const bool& isEchoing, const int threshold, QObject *parent=0);
  virtual ~TestServer();

private slots:

  void Start();
  void OnSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);
  void OnMessageReceived(int portNumber, NiftyLinkMessageContainer::Pointer message);
  void OnClientConnected(int portNumber);

private:
  int                 m_PortNumber;
  bool                m_IsEchoing;
  int                 m_StatsThreshold;
  NiftyLinkTcpServer *m_Server;
  QTimer             *m_StatsTimer;
};

} // end namespace niftk

#endif // TestServer_h
