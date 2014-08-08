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
#include <NiftyLinkQThread.h>

#include <QObject>
#include <QTcpSocket>

#include <igtlMessageBase.h>

namespace niftk
{

class NiftyLinkTcpNetworkWorker;

/**
 * \class NiftyLinkTcpClient
 * \brief TCP client that runs a QTcpSocket via a NiftyLinkTcpNetworkWorker
 * in another NiftyLinkQThread, sending and receiving OpenIGTLink messages.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpClient : public QObject
{
  Q_OBJECT

public:
  NiftyLinkTcpClient(QObject *parent = 0);
  virtual ~NiftyLinkTcpClient();

  /// \brief Set a threshold for the number of messages, so that you
  /// get stats every X number of messages. Set <em>threshold</em>
  /// to -1 to turn this feature off. Defaults to off.
  void SetNumberMessageReceivedThreshold(qint64 threshold);

  /// \brief Set this object to either send or not send keep alive messages.
  /// Use in conjunction with SetCheckForNoIncomingData().
  /// eg. One end sends keep alive messages, the other end expects to receive data regularly.
  void SetKeepAliveOn(bool isOn);

  /// \brief Set this object to monitor for no incoming data.
  /// Use in conjunction with SetKeepAliveOn().
  /// eg. One end sends keep alive messages, the other end expects to receive data regularly.
  void SetCheckForNoIncomingData(bool isOn);

  /// \brief Connects to a host.
  ///
  /// You should register and listen to SocketError signal before calling this.
  void ConnectToHost(const QString& hostName, quint16 portNumber);

  /// \brief Disconnects.
  void DisconnectFromHost();

  /// \brief Returns true if connected and false otherwise.
  bool IsConnected() const;

  /// \brief Sends an OpenIGTLink message.
  ///
  /// The OpenIGTLink message within NiftyLinkMessageContainer should be Packed.
  /// \return false if socket closed or unwritable, true otherwise.
  bool Send(NiftyLinkMessageContainer::Pointer message);

public slots:

  /// \brief Writes some stats to console, see NiftyLinkMessageCounter.
  /// Defined as a slot, so we can trigger it via QTimer for instance.
  void OutputStats();

  /// \brief Sends message to other end to request the other end to output stats to console.
  /// Defined as a slot, so we can trigger it via QTimer for instance.
  void RequestStats();

signals:

  /// \brief Emmitted when we have successfully connected.
  void Connected(QString hostName, int portNumber);

  /// \brief Emmitted when we have disconnected for any reason.
  void Disconnected(QString hostName, int portNumber);

  /// \brief Emitted when the underlying socket reports an error.
  void SocketError(QString hostName, int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief Emitted by this class when an error has occured (eg. usage error).
  void ClientError(QString hostName, int portNumber, QString errorString);

  /// \brief Emitted when this client receives an OpenIGTLink message, messages come out UnPacked.
  /// IMPORTANT: You must use a Qt::DirectConnection to connect to this, and not a Qt::QueuedConnection.
  void MessageReceived(NiftyLinkMessageContainer::Pointer message);

  /// \brief Emitted by the underlying socket when we have actually sent bytes.
  void BytesSent(qint64 bytes);

  /// \brief Emmitted when a keep alive message was sent.
  void SentKeepAlive();

  /// \brief Emmitted when the other end is failing to send data.
  void NoIncomingData();

  /// \brief Internal use only.
  void InternalCloseSocketSignal();

    /// \brief Internal use only.
  void InternalShutdownThreadSignal();

private slots:

  /// \brief When the socket successfully connects, we move all processing to another thread.
  void OnConnected();

  /// \brief At the moment, just log, and emmit this class's Disconnected signal.
  void OnDisconnected();

  /// \brief We need to listen to all socket errors from the moment we create it.
  void OnError();

  /// \brief Once the worker takes over, we listen to his reports of the socket errors.
  void OnWorkerSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief At the moment, this just copes with the different number of arguments, and passes the message on.
  void OnMessageReceived(int portNumber);

private:

  void RaiseInternalError(const QString& errorMessage);

  QTcpSocket                *m_Socket;
  NiftyLinkTcpNetworkWorker *m_Worker;
  NiftyLinkQThread          *m_Thread;
  QString                    m_RequestedName;
  int                        m_RequestedPort;
  NiftyLinkMessageManager    m_InboundMessages;
  NiftyLinkMessageManager    m_OutboundMessages;
  bool                       m_Connected;

}; // end class

} // end namespace niftk

#endif
