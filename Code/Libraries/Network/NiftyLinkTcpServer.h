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

#include <QTcpServer>
#include <QSet>
#include <QMutex>

class QTcpSocket;

namespace niftk
{
/**
 * \class NiftyLinkTcpServer
 * \brief TCP server that processes multiple clients, each in a separate
 * QThread, sending and receiving OpenIGTLink messages.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpServer : public QTcpServer
{
  Q_OBJECT

public:
  NiftyLinkTcpServer(QObject *parent = 0);
  virtual ~NiftyLinkTcpServer();

protected:

  // Override the base class method.
  virtual void incomingConnection(int socketDescriptor);

signals:

  /// \brief Emmitted when a remote client connects.
  void ClientConnected(int portNumber);

  /// \brief Emmitted when a remote client disconnects.
  void ClientDisconnected(int portNumber);

  /// \brief Emitted when a new message is received from a remote client.
  void MessageReceived(int portNumber, niftk::NiftyLinkMessageContainer::Pointer msg);

  /// \brief Emitted when one of the remote clients reports an error.
  void SocketError(int portNumber, QAbstractSocket::SocketError socketError);

private slots:

  void OnClientConnected();
  void OnClientDisconnected();

private:

  QSet<QTcpSocket*> m_Sockets;
  QMutex            m_Mutex;
};

} // end namespace niftk

#endif // NiftyLinkTcpServer_h
