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
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpNetworkWorker : public QObject
{
  Q_OBJECT

public:

  NiftyLinkTcpNetworkWorker(QTcpSocket *socket, QObject *parent = 0);
  virtual ~NiftyLinkTcpNetworkWorker();

public slots:

  /// \brief Sends an OpenIGTLink message, written as a slot, as it may be called from another thread.
  void Send(igtl::MessageBase::Pointer msg);

signals:

  void SocketError(int portNumber, QAbstractSocket::SocketError);
  void MessageReceived(int portNumber, niftk::NiftyLinkMessageContainer::Pointer msg);
  void BytesSent(qint64 bytes);
  void InternalSendSignal(igtl::MessageBase::Pointer msg);

private slots:

  void OnSocketDisconnected();
  void OnSocketError(QAbstractSocket::SocketError error);
  void OnSocketReadyRead();
  void OnSend(igtl::MessageBase::Pointer msg);

private:
  QTcpSocket                   *m_Socket;
  QString                       m_MessagePrefix;
  bool                          m_HeaderInProgress;
  bool                          m_MessageInProgress;
  igtl::MessageHeader::Pointer  m_IncomingHeader;
  igtl::TimeStamp::Pointer      m_IncomingHeaderTimeStamp;
  igtl::MessageBase::Pointer    m_IncomingMessage;
  quint64                       m_IncomingMessageBytesReceived;
  bool                          m_AbortReading;
}; // end class

} // end namespace niftk

#endif // NiftyLinkTcpNetworkWorker_h
