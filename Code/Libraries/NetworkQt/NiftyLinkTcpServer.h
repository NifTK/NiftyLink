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
#include <NiftyLinkMessageCounter.h>
#include <NiftyLinkTcpNetworkWorker.h>

#include <QSet>
#include <QMutex>
#include <QTcpServer>

namespace niftk
{
/**
* \class NiftyLinkTcpServer
* \brief TCP server that processes multiple clients bound to a single port,
* running each in a separate NiftyLinkQThread using NiftyLinkTcpNetworkWorker,
* sending and receiving OpenIGTLink messages.
*
* Lots of functionality is provided by the QTcpServer base class.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpServer : public QTcpServer
{
  Q_OBJECT

public:

  NiftyLinkTcpServer(QObject *parent = 0);

  /// \brief Destroy the server.
  ///
  /// If there are clients connected, this method will call Shutdown() first.
  virtual ~NiftyLinkTcpServer();

  /// \brief Call this to shut down.
  ///
  /// Will ask each connected client to disconnect, and will wait until
  /// all the connections have been disconnected, after which it is safe to
  /// destroy this object.
  void Shutdown();

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

  /// \brief Returns the number of connected clients.
  int GetNumberOfClientsConnected();

  /// \brief Sends an OpenIGTLink message to all connected clients.
  /// \return the number of clients sent to.
  /// The OpenIGTLink message within NiftyLinkMessageContainer should be Packed.
  int Send(NiftyLinkMessageContainer::Pointer message);

public slots:

  /// \brief Writes some stats to console, see NiftyLinkMessageCounter.
  ///
  /// Defined as a slot, so we can trigger it via QTimer.
  /// This outputs from all stats counters for each worker.
  void OutputStats();

signals:

  /// \brief Emmitted when a remote client connects.
  void ClientConnected(int portNumber);

  /// \brief Emmitted when a remote client disconnects.
  void ClientDisconnected(int portNumber);

  /// \brief Emitted when one of the remote clients reports an error.
  void SocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief Emitted when a new message is received from a remote client, messages are UnPacked.
  /// IMPORTANT: Use a Qt::DirectConnection, and never a Qt::QueuedConnection.
  void MessageReceived(int portNumber, NiftyLinkMessageContainer::Pointer message);

  /// \brief Emmitted when we have actually sent bytes.
  void BytesSent(qint64 bytes);

  /// \brief Emmitted when a keep alive message was sent.
  void SentKeepAlive();

  /// \brief Emmitted when any of the connected clients is failing to send data.
  void NoIncomingData();

  /// \brief Emmitted when we are starting to shut things down.
  void StartShutdown();

  /// \brief Emmitted when we have finished all the shut down stuff.
  void EndShutdown();

  /// \brief Emmitted every time stats were computed.
  void StatsProduced(NiftyLinkMessageStatsContainer stats);

  /// \brief Emmitted every time stats were computed.
  void StatsMessageProduced(QString stringRepresentation);

protected:

  // Override the base class method.
  virtual void incomingConnection(int socketDescriptor);

private slots:

  void OnClientDisconnected();
  void OnMessageReceived(int portNumber);

private:

  QSet<NiftyLinkTcpNetworkWorker*> m_Workers;
  QMutex                           m_Mutex;
  NiftyLinkMessageManager          m_InboundMessages;
  NiftyLinkMessageManager          m_OutboundMessages;
  NiftyLinkMessageCounter          m_ReceivedCounter;
  bool                             m_SendKeepAlive;
  bool                             m_CheckNoIncoming;
};

} // end namespace niftk

#endif // NiftyLinkTcpServer_h
