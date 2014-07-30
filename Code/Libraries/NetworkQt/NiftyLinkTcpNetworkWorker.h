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

namespace niftk
{
/**
 * \class NiftyLinkTcpNetworkWorker
 * \brief Worker object, to be run in a separate NiftyLinkQThread by NiftyLinkTcpServer or NiftyLinkTcpClient.
 *
 * Third parties should not need to use this class, hence it is deliberately not exported.
 */
class NiftyLinkTcpNetworkWorker : public QObject
{
  Q_OBJECT

public:

  /// \brief Constructor, and this class owns none of the objects passed in, so won't destroy them.
  NiftyLinkTcpNetworkWorker(NiftyLinkMessageManager* inboundMessages,
                            NiftyLinkMessageManager* outboundMessages,
                            QTcpSocket *socket,
                            QObject *parent = 0);

  /// \brief Destructor.
  virtual ~NiftyLinkTcpNetworkWorker();

  /// \brief Returns the contained socket.
  QTcpSocket* GetSocket() const;

  /// \brief Closes the provided socket.
  static bool CloseSocket(QTcpSocket* );

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

  /// \brief Called from within OutputStatsToConsole(), and will also send
  /// a message via the socket to request stats at the other end.
  /// \return false if socket closed or unwritable, true otherwise.
  bool RequestStats();

  /// \brief Sends a request to output some statistics to console.
  void OutputStatsToConsole();

signals:

  void SocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);
  void MessageReceived(int portNumber);
  void BytesSent(qint64 bytes);
  void SentKeepAlive();
  void NoIncomingData();
  void InternalSendSignal();
  void InternalStatsSignal();

private slots:

  void OnSocketDisconnected();
  void OnSocketError(QAbstractSocket::SocketError error);
  void OnOutputStats();
  void OnCheckForIncomingData();

  void OnSocketReadyRead();
  void OnSend();
  void OnSendInternalPing();

private:

  void SendMessage(igtl::MessageBase::Pointer);

  NiftyLinkMessageManager      *m_InboundMessages;
  NiftyLinkMessageManager      *m_OutboundMessages;
  QTcpSocket                   *m_Socket;
  QString                       m_MessagePrefix;

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

}; // end class

} // end namespace niftk

#endif // NiftyLinkTcpNetworkWorker_h
