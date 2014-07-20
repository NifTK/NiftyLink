/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkClient_h
#define NiftyLinkClient_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>

#include <igtlMessageBase.h>

#include <QObject>

namespace niftk
{

class NiftyLinkQThread;
class NiftyLinkClientProcess;

/**
 * \class NiftyLinkClient
 * \brief Main NiftyLink Client class that users instantiate to
 * connect to a local/remote host and port start to send messages.
 *
 * Note: Error handling strategy is to throw std::exception sub-classes for everything.
 */
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkClient : public QObject {

  Q_OBJECT

  friend class NiftyLinkClientProcess;

public:

  /// Constuctor.
  NiftyLinkClient(QObject *parent = 0);

  /// Destructor.
  ~NiftyLinkClient();

  /// \brief Starts a client, and tries to connect to the given URL.
  void Start(const QUrl& url);

  /// \brief Sends an OpenIGTLink message (blocking).
  void Send(igtl::MessageBase::Pointer msg);

  /// \brief Stops this client.
  void Stop();

  /// \brief Sets the time to wait when trying to connect to server, default = 5000 msec.
  void SetTimeToWaitToConnectToServer(const int& msec);

  /// \brief Returns the time to wait when trying to connect to the server.
  int GetTimeToWaitToConnectToServer();

signals:

  /// \brief This signal is emitted when the client first connects to the remote server.
  void ConnectedToServer();

  /// \brief This signal is emitted when the client disconnects from the remote server.
  /// This is determined by failure to send a keep-alive ping.
  void DisconnectedFromServer();

  /// \brief This signal is emmitted when this client thinks it has not heard from the server in a while.
  /// This is determined by the lack of incoming data, such as keep-alive pings from the server.
  void ServerDisconnected();

  /// \brief This signal is emitted when a new message is received from the remote server.
  void MessageReceived(NiftyLinkMessageContainer::Pointer msg);

  /// \brief This signal is emmitted when a message has been successfully sent.
  /// The start and end times indicate the time it took to process the Send() request.
  void MessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

private slots:

  /// See QThread manual page, this slot is used to launch processing in another worker thread.
  void OnStartProcessing();

  // To process signals from NiftyLinkClientProcess, which are just propagated.
  void OnProcessesConnectedToRemote();
  void OnMessageReceived(NiftyLinkMessageContainer::Pointer msg);
  void OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);
  void OnCantReachRemote();
  void OnNoResponseFromRemote();

private:

  NiftyLinkQThread       *m_Thread;
  NiftyLinkClientProcess *m_ClientProcess;
};

} // end namespace niftk

#endif
