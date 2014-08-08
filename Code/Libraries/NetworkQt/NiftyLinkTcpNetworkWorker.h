/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTcpNetworkWorker_h
#define NiftyLinkTcpNetworkWorker_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkMessageManager.h>
#include <NiftyLinkMessageCounter.h>
#include <igtlMessageBase.h>
#include <igtlTimeStamp.h>

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QWaitCondition>

namespace niftk
{
/**
 * \class NiftyLinkTcpNetworkWorker
 * \brief Worker object, to be run in a separate NiftyLinkQThread by NiftyLinkTcpServer or NiftyLinkTcpClient.
 *
 * Third parties should not need to use this class, hence it is deliberately not exported.
 *
 * Once the this worker is running in its own event loop, you should consider the socket
 * and worker as being owned by the thread, and events are fired from the event loop of the new thread.
 */
class NiftyLinkTcpNetworkWorker : public QObject
{
  Q_OBJECT

public:

  /// \brief Constructor.
  NiftyLinkTcpNetworkWorker(NiftyLinkMessageManager* inboundMessages,
                            NiftyLinkMessageManager* outboundMessages,
                            QTcpSocket *socket,
                            QObject *parent = 0);

  /// \brief Destructor.
  virtual ~NiftyLinkTcpNetworkWorker();

  /// \brief Returns the contained socket, but breaks encapsulation - use carefully.
  QTcpSocket* GetSocket() const;

  /// \brief Returns true if the socket exists, and the socket says its open, and false otherwise.
  bool IsSocketConnected() const;

  /// \brief Requests the socket is disconnected.
  /// This will be processed by the event loop containing this object.
  void RequestDisconnectSocket();

  /// \brief Set a threshold for the number of messages, so that you
  /// get stats printed to console every X number of messages. Set to -1 to turn this off.
  void SetNumberMessageReceivedThreshold(qint64 threshold);

  /// \brief Set this object to either send or not send keep alive messages.
  /// Use in conjunction with SetCheckForNoIncomingData().
  /// eg. One end sends keep alive messages, the other end expects to receive data regularly.
  void SetKeepAliveOn(bool isOn);

  /// \brief Set this object to monitor for no incoming data.
  /// Use in conjunction with SetKeepAliveOn().
  /// eg. One end sends keep alive messages, the other end expects to receive data regularly.
  void SetCheckForNoIncomingData(bool isOn);

  /// \brief Sends an OpenIGTLink message.
  /// The OpenIGTLink message within NiftyLinkMessageContainer should be already Packed.
  /// \return false if socket closed or unwritable, true otherwise.
  bool Send(NiftyLinkMessageContainer::Pointer message);

  /// \brief Queues a request to output some statistics to console.
  void OutputStatsToConsole();

  /// \brief Called from within OutputStatsToConsole(), to send
  /// a message via the socket to request stats at the other end.
  /// \return false if socket closed or unwritable, true otherwise.
  bool RequestStats();

signals:

  /// \brief The socket has disconnected, which means everything has commenced shutdown.
  void SocketDisconnected();

  /// \brief Reports error messages from socket.
  void SocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief Emitted when a message is ready for collection.
  void MessageReceived(int portNumber);

  /// \brief Emitted when bytes have actually been transmitted.
  void BytesSent(qint64 bytes);

  /// \brief Only emitted when we have explicitly enabled this.
  void SentKeepAlive();

  /// \brief Only emitted when we are explicitly checking for this.
  void NoIncomingData();

  /// \brief Internal use only.
  void InternalSendSignal();

  /// \brief Internal use only.
  void InternalStatsSignal();

  /// \brief Internal use only.
  void InternalDisconnectedSocketSignal();

private slots:

  /// \brief Internal slot that actually tells the socket to disconnect.
  void OnRequestSocketDisconnected();

  /// \brief Triggered by socket when disconnected.
  /// This has knock on effects, effectively scheduling the socket, the worker (this class), and the thread to shutdown and delete.
  void OnSocketDisconnected();

  /// \brief Receives error messages from socket.
  void OnSocketError(QAbstractSocket::SocketError error);

  /// \brief Outputs stats to console.
  /// Due to multi-threaded nature, should only be called by the main thread owning this class.
  void OnOutputStats();

  /// \brief Triggered by internal timer, and will repeatedly indicate whether any incoming data is being received.
  void OnCheckForIncomingData();

  /// \brief Checks if there is any data left to write, and signals on m_WaitForSend, and passes BytesSent(qint64) on.
  void OnBytesSent(qint64);

  /// \brief Reads data from socket, should only be triggered by socket.
  /// Due to multi-threaded nature, should only be called by the main thread owning this class.
  void OnSocketReadyRead();

  /// \brief Sends OpenIGTLink message from holding bay.
  /// Due to multi-threaded nature, should only be called by the main thread owning this class.
  void OnSendMessage();

  /// \brief Sends STATUS_OK message.
  /// Due to multi-threaded nature, should only be called by the main thread owning this class.
  void OnSendInternalPing();

private:

  /// \brief Actually sends a message out the socket, so don't expose this publically.
  void InternalSendMessage(igtl::MessageBase::Pointer);

  /// \brief Asks the containing thread to quit.
  void ShutdownThread();

  QTcpSocket                   *m_Socket;
  QString                       m_MessagePrefix;

  // Holding bays so we dont pass NiftyLinkMessageContainer over signals/slots and accidentally copy it.
  NiftyLinkMessageManager      *m_InboundMessages;
  NiftyLinkMessageManager      *m_OutboundMessages;

  // For parsing fractions of message.
  bool                          m_HeaderInProgress;
  bool                          m_MessageInProgress;
  igtl::MessageHeader::Pointer  m_IncomingHeader;
  igtl::TimeStamp::Pointer      m_IncomingHeaderTimeStamp;
  igtl::TimeStamp::Pointer      m_TimeFullyReceivedTimeStamp;
  igtl::MessageBase::Pointer    m_IncomingMessage;
  quint64                       m_IncomingMessageBytesReceived;
  bool                          m_AbortReading;

  // For stats.
  NiftyLinkMessageCounter       m_ReceivedCounter;

  // For internal 'keep-alive' message
  QTimer                        *m_KeepAliveTimer;
  int                            m_KeepAliveInterval;
  igtl::TimeStamp::Pointer       m_KeepAliveTimeStamp;
  igtl::TimeStamp::Pointer       m_LastMessageSentTime;

  // For monitoring for no incoming data.
  QTimer                        *m_NoIncomingDataTimer;
  int                            m_NoIncomingDataInterval;
  igtl::TimeStamp::Pointer       m_NoIncomingDataTimeStamp;
  igtl::TimeStamp::Pointer       m_LastMessageReceivedTime;

  // Disconnecting in progress.
  bool                           m_Disconnecting;

}; // end class

} // end namespace niftk

#endif // NiftyLinkTcpNetworkWorker_h
