/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkApp_h
#define NiftyLinkApp_h

#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkMessageCounter.h>
#include <NiftyLinkMessageManager.h>
#include <QMainWindow>
#include <QAbstractSocket>
#include <QTimer>
#include "ui_NiftyLink.h"

namespace niftk
{

class NiftyLinkTcpClient;

/**
 * \brief NiftyLinkApp is a demo app to connect and stream data between two server nodes.
 *
 * It was created because PLUS starts up a server. So you can use PLUS to connect
 * to various ultrasound machines, and tracking devices. You then need to connect
 * a client.
 *
 * NiftyView however also starts up a server, as it assumes all things connected
 * to it (via OpenIGTLink at least) will be clients sending data.
 *
 * So, this program functions as a simple bridge between the two. It could
 * also be extended to have multiple inbound sockets, multiple outbound
 * sockets, QRegExp for deciding which message types to forward, and then
 * it could function like an OpenIGTLink message router.
 *
 * All the controls and functionality are visible in the GUI, and
 * this class is the main application entry point. There is currently
 * no public API because there is nothing to set.
 */
class NiftyLinkApp : public QMainWindow, public Ui_NiftyLink {

  Q_OBJECT

public:
  NiftyLinkApp(QObject *parent = 0);
  virtual ~NiftyLinkApp();

private slots:

  /// \brief When the inbound connect button is pressed we pass the host and port to the socket, and ask it to connect.
  void OnConnectInboundButtonPressed();

  /// \brief When the inbound client connects we disable the connect button, enable the disconnect button.
  void OnConnectedInbound(QString hostName, int portNumber);

  /// \brief When the inbound disconnect button is pressed we ask the socket to disconnect.
  void OnDisconnectInboundButtonPressed();

  /// \brief When the inbound client disconnects we enable the connect button, disable the disconnect button.
  void OnDisconnectedInbound(QString hostName, int portNumber);

  /// \brief When the outbound connect button is pressed we pass the host and port to the socket, and ask it to connect.
  void OnConnectOutboundButtonPressed();

  /// \brief When the outbound client connects we disable the connect button, enable the disconnect button.
  void OnConnectedOutbound(QString hostName, int portNumber);

  /// \brief When the outbound disconnect button is pressed we ask the socket to disconnect.
  void OnDisconnectOutboundButtonPressed();

  /// \brief When the outbound client disconnects we enable the connect button, disable the disconnect button.
  void OnDisconnectedOutbound(QString hostName, int portNumber);

  /// All socket error messages are raised here.
  void OnSocketError(QString hostName, int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief When a message is received from the inbound socket.
  void OnMessageReceived(NiftyLinkMessageContainer::Pointer message);

  /// \brief Called by timer to update the status bar once per second.
  void OnUpdateStatus();

  /// \brief Called by timer to update the screen, up to 33.33 times per second.
  void OnUpdateScreen();

private:

  NiftyLinkTcpClient      *m_InboundClient;
  NiftyLinkTcpClient      *m_OutboundClient;
  QImage                   m_MostRecentImage;
  QString                  m_MostRecentString;
  NiftyLinkMessageCounter  m_MessagesReceived;
  NiftyLinkMessageCounter  m_MessagesSent;
  QTimer                  *m_ScreenTimer;
  QTimer                  *m_StatusTimer;
};

} // end namespace

#endif // NiftyLink_h
