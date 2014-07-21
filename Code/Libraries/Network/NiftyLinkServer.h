/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkServer_h
#define NiftyLinkServer_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlMessageBase.h>

#include <QObject>

namespace niftk
{

class NiftyLinkQThread;
class NiftyLinkServerProcess;

/**
 * \class NiftyLinkServer
 * \brief Main NiftyLink Server class that users instantiate to
 * bind to a socket and start to listen for messages.
 *
 * Note: Error handling strategy is to throw std::exception sub-classes for everything.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkServer : public QObject {

  Q_OBJECT

  friend class NiftyLinkServerProcess;

public:

  /// Constructor.
  NiftyLinkServer(QObject *parent = 0);

  /// Destructor.
  ~NiftyLinkServer();

  /// \brief Starts the server, and binds to a specific port number.
  void Start(const int& portNumber);

  /// \brief Sends an OpenIGTLink message (blocking).
  void Send(igtl::MessageBase::Pointer msg);

  /// \brief Stops this server.
  void Stop();

  /// \brief Sets the time to wait for clients to connect, default = 2000 msec.
  /// This class also loops round repeatedly checking this, so its not critical.
  void SetTimeToWaitForClientConnections(const int& msec);

  /// \brief Returns the time to wait for client connections.
  int GetTimeToWaitForClientConnections();

signals:

  /// \brief This signal is emitted when a client connects to this server.
  void ClientConnected();

  /// \brief This signal is emitted when the server can't send a keep-alive message to the remote client.
  void FailedToSendKeepAliveMessage();

  /// \brief This signal is emmitted when this server thinks it has not heard from the client in a while.
  void NoIncommingData();

  /// \brief This signal is emitted when a new message is received from the remote peer.
  void MessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg);

  /// \brief This signal is emmitted when a message has been successfully sent.
  /// The start and end times indicate the time it took to process the Send() request.
  void MessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

private slots:

  /// See QThread manual page, this slot is used to launch processing in another worker thread.
  void StartProcessing();

  // To process signals from NiftyLinkServerProcess, which are just propagated.
  void OnClientConnected();
  void OnCantSendToClient();
  void OnClientDisconnected();
  void OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg);
  void OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

private:

  NiftyLinkQThread       *m_Thread;
  NiftyLinkServerProcess *m_ServerProcess;
};

} // end namespace niftk

#endif
