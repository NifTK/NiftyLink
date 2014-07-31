/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLink_h
#define NiftyLink_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkMessageCounter.h>
#include <igtlTimeStamp.h>
#include <QMainWindow>
#include <QAbstractSocket>
#include <QTimer>
#include "ui_NiftyLink.h"

namespace niftk
{

class NiftyLinkTcpClient;

/**
 * \brief NiftyLink is a demo app to connect and stream data between two server nodes.
 */
class NiftyLink : public QMainWindow, public Ui_NiftyLink {

  Q_OBJECT

public:
  NiftyLink(QObject *parent = 0);
  virtual ~NiftyLink();

private slots:

  void OnConnectInboundCalled();
  void OnConnectedInbound(QString hostName, int portNumber);
  void OnDisconnectInboundCalled();
  void OnDisconnectedInbound(QString hostName, int portNumber);

  void OnConnectOutboundCalled();  
  void OnConnectedOutbound(QString hostName, int portNumber);
  void OnDisconnectOutboundCalled();
  void OnDisconnectedOutbound(QString hostName, int portNumber);

  void OnSocketError(QString hostName, int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);
  void OnMessageReceived(NiftyLinkMessageContainer::Pointer message);
  void OnUpdateStatus();

private:

  NiftyLinkTcpClient      *m_InboundClient;
  NiftyLinkTcpClient      *m_OutboundClient;
  NiftyLinkMessageCounter  m_MessagesReceived;
  NiftyLinkMessageCounter  m_MessagesSent;
  QTimer                  *m_Timer;
};

} // end namespace

#endif // NiftyLink_h
