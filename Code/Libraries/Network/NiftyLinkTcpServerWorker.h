/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkTcpServerWorker_h
#define NiftyLinkTcpServerWorker_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlMessageBase.h>
#include <igtlTimeStamp.h>

#include <QThread.h>
#include <QTcpSocket>

namespace niftk
{
/**
 * \class NiftyLinkTcpServerWorker
 * \brief Worker object, to be run in a separate QThread by NiftyLinkTcpServer.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpServerWorker : public QObject
{
  Q_OBJECT

public:

  NiftyLinkTcpServerWorker(QTcpSocket *socket, QObject *parent = 0);
  virtual ~NiftyLinkTcpServerWorker();

signals:

  void SocketError(int portNumber, QAbstractSocket::SocketError);
  void MessageReceived(int portNumber, niftk::NiftyLinkMessageContainer::Pointer msg);

private slots:

  void OnSocketDisconnected();
  void OnSocketError(QAbstractSocket::SocketError error);
  void OnSocketReadyRead();

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

}

#endif // NiftyLinkTcpServerWorker_h
