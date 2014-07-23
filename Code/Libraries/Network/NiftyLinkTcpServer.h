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
#include <NiftyLinkTcpNetworkWorker.h>

#include <QSet>
#include <QMutex>
#include <QTcpServer>

namespace niftk
{
/**
 * \class NiftyLinkTcpServer
 * \brief TCP server that processes multiple clients bound to a single port,
 * each in a separate QThread, sending and receiving OpenIGTLink messages.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkTcpServer : public QTcpServer
{
  Q_OBJECT

public:

  NiftyLinkTcpServer(QObject *parent = 0);
  virtual ~NiftyLinkTcpServer();

  /// \brief Sends an OpenIGTLink message to all clients.
  void Send(igtl::MessageBase::Pointer msg);

  /// \brief Writes some stats to console.
  void OutputStats();

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
  void SocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString);

  /// \brief Emmitted when we are starting to shut things down.
  void StartShutdown();

  /// \brief Emmitted when we have finished all the clear down stuff.
  void EndShutdown();

private slots:

  void OnClientConnected();
  void OnClientDisconnected();

private:

  void Shutdown();

  QSet<NiftyLinkTcpNetworkWorker*> m_Workers;
  QMutex                           m_Mutex;
};

} // end namespace niftk

#endif // NiftyLinkTcpServer_h
