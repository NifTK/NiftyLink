/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTcpServer_h
#define NiftyLinkTcpServer_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkMessageManager.h>

#include <QObject>
#include <QTcpSocket>

#include <igtlMessageBase.h>

namespace niftk
{

class NiftyLinkTcpNetworkWorker;

/**
 * \class NiftyLinkTcpClient
 * \brief TCP client that runs a QTcpSocket in another QThread,
 * sending and receiving OpenIGTLink messages.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpClient : public QObject
{
  Q_OBJECT

public:
  NiftyLinkTcpClient(QObject *parent = 0);
  virtual ~NiftyLinkTcpClient();

  /// \brief Connects to a host.
  ///
  /// You should register and listen to SocketError signal before calling this.
  void ConnectToHost(const QString& hostName, quint16 portNumber);

  /// \brief Sends an OpenIGTLink message.
  ///
  /// The OpenIGTLink message within NiftyLinkMessageContainer should be Packed.
  void Send(NiftyLinkMessageContainer::Pointer message);

  /// \brief Set a threshold for the number of messages, so that you
  /// get stats every X number of messages. Set to -1 to turn this off.
  /// Defaults to off.
  void SetNumberMessageReceivedThreshold(qint64 threshold);

  /// \brief Set this object to either send or not send keep alive messages.
  void SetKeepAliveOn(bool isOn);

public slots:

  /// \brief Writes some stats to console.
  /// Defined as a slot, so we can trigger it via QTimer.
  void OutputStats();

signals:

  /// \brief Emmitted when we have successfully connected.
  void Connected();

  /// \brief Emmitted when we have disconnected for any reason.
  void Disconnected();

  /// \brief Emitted when the underlying socket reports an error.
  void SocketError(QString hostName, int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief Emitted when this client receives an OpenIGTLink message, messages come out UnPacked.
  /// IMPORTANT: You must use a Qt::DirectConnection to connect to this.
  void MessageReceived(NiftyLinkMessageContainer::Pointer message);

  /// \brief Emmitted by the underlying socket when we have actually sent bytes.
  void BytesSent(qint64 bytes);

  /// \brief Emmitted when a keep alive message was sent.
  void SentKeepAlive();

private slots:

  /// \brief When the socket successfully connects, we move all processing to another thread.
  void OnConnected();

  /// \brief We listen to all socket errors from the moment we create it.
  void OnError();

  /// \brief At the moment, just log, and emmit the Disconnected signal.
  void OnDisconnected();

  /// \brief At the moment, this just copes with the different number of arguments, and passes the message on.
  void OnMessageReceived(int portNumber);

private:

  QTcpSocket                *m_Socket;
  NiftyLinkTcpNetworkWorker *m_Worker;
  NiftyLinkMessageManager    m_InboundMessages;
  NiftyLinkMessageManager    m_OutboundMessages;

}; // end class

} // end namespace niftk

#endif
