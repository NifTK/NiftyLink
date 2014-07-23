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

  /// \brief Returns the contained socket, (but breaks encapsulation)!
  QTcpSocket* GetTcpSocket() const;

  /// \brief Connects to a host, before calling this, register for all errors on the above mentioned socket.
  void ConnectToHost(const QString& hostName, quint16 portNumber);

  /// \brief Sends an OpenIGTLink message.
  void Send(igtl::MessageBase::Pointer msg);

signals:

  /// \brief Emitted when this client receives an OpenIGTLink message.
  void MessageReceived(int portNumber, niftk::NiftyLinkMessageContainer::Pointer msg);

  /// \brief Emitted when the socket reports an error.
  void SocketError(int portNumber, QAbstractSocket::SocketError socketError);

  /// \brief Emmitted when we have actually sent bytes.
  void BytesSent(qint64 bytes);

private slots:

  /// \brief When the socket successfully connects, we move all processing to another thread.
  void OnConnected();

private:

  QTcpSocket                *m_Socket;
  NiftyLinkTcpNetworkWorker *m_Worker;

}; // end class

} // end namespace niftk

#endif
