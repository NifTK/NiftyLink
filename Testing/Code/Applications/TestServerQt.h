/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestServerQt_h
#define TestServerQt_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkTcpServer.h>

#include <igtlTimeStamp.h>

#include <QObject>
#include <QTimer>

namespace niftk
{

class TestServerQt : public QObject
{

  Q_OBJECT

public:
  TestServerQt(const int& portNumber,
               const bool& isEchoing,
               const bool& doStats,
               const int threshold,
               QObject *parent=0);
  virtual ~TestServerQt();

private slots:

  void Start();
  void Stop();
  void OnSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);
  void OnMessageReceived(int portNumber, NiftyLinkMessageContainer::Pointer message);
  void OnClientConnected(int portNumber);
  void OnKeepAliveSent();
  void OnNoIncomingData();

private:
  int                 m_PortNumber;
  bool                m_IsEchoing;
  bool                m_DoStats;
  int                 m_StatsThreshold;
  NiftyLinkTcpServer *m_Server;
  QTimer             *m_StatsTimer;
};

} // end namespace niftk

#endif // TestServerQt_h
