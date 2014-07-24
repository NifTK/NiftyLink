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
#include <igtlMessageBase.h>
#include <igtlTimeStamp.h>

#include <QThread>
#include <QTcpSocket>

namespace niftk
{
/**
 * \class NiftyLinkTcpNetworkWorker
 * \brief Worker object, to be run in a separate QThread by NiftyLinkTcpServer or NiftyLinkTcpClient.
 */
class NiftyLinkTcpNetworkWorker : public QObject
{
  Q_OBJECT

public:

  NiftyLinkTcpNetworkWorker(NiftyLinkMessageManager* inboundMessages,
                            NiftyLinkMessageManager* outboundMessages,
                            QTcpSocket *socket,
                            QObject *parent = 0);
  virtual ~NiftyLinkTcpNetworkWorker();

  /// \brief Returns the contained socket - be careful, breaks encapsulation.
  QTcpSocket* GetSocket() const;

  /// \brief Sends an OpenIGTLink message.
  /// The OpenIGTLink message within NiftyLinkMessageContainer should be already packed.
  void Send(NiftyLinkMessageContainer::Pointer message);

  /// \brief Sends a request to output some statistics to console.
  void OutputStatsToConsole();

signals:

  void SocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);
  void MessageReceived(int portNumber);
  void BytesSent(qint64 bytes);
  void InternalSendSignal();
  void InternalStatsSignal();

private slots:

  void OnSocketDisconnected();
  void OnSocketError(QAbstractSocket::SocketError error);
  void OnSocketReadyRead();
  void OnSend();
  void OnOutputStats();

private:
  NiftyLinkMessageManager      *m_InboundMessages;
  NiftyLinkMessageManager      *m_OutboundMessages;
  QTcpSocket                   *m_Socket;
  QString                       m_MessagePrefix;
  bool                          m_HeaderInProgress;
  bool                          m_MessageInProgress;
  igtl::MessageHeader::Pointer  m_IncomingHeader;
  igtl::TimeStamp::Pointer      m_IncomingHeaderTimeStamp;
  igtl::MessageBase::Pointer    m_IncomingMessage;
  quint64                       m_IncomingMessageBytesReceived;
  bool                          m_AbortReading;
  igtl::TimeStamp::Pointer      m_StatsTimePoint;
  quint64                       m_TotalBytesReceived;
  quint64                       m_NumberMessagesReceived;
  quint64                       m_NumberMessagesSent;
  QList<quint64>                m_ListOfLatencies;

}; // end class

} // end namespace niftk

#endif // NiftyLinkTcpNetworkWorker_h
