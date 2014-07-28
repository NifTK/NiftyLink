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
 * \brief TCP Client class that users instantiate to connect to a
 * local/remote host and port, and send OpenIGTLink messages.
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

  /// \brief Starts a client, and tries to connect to the given host and port.
  void ConnectToHost(const QString& hostName, quint16 portNumber);

  /// \brief Starts a client, and tries to connect to the given URL.
  void ConnectToHost(const QUrl& url);

  /// \brief Sends an OpenIGTLink message (blocking).
  void Send(igtl::MessageBase::Pointer msg);

  /// \brief Stops this client. This ultimately causes the thread to die, so a new client should be created after calling this.
  void Stop();

  /// \brief Sets the time to wait when trying to connect to server, default = 5000 msec.
  void SetTimeToWaitToConnectToServer(const int& msec);

  /// \brief Returns the time to wait when trying to connect to the server.
  int GetTimeToWaitToConnectToServer();

  /// \brief Used for testing, and performance analysis, writing some stats to console.
  void OutputStats();

signals:

  /// \brief This signal is emitted when the client first connects to the remote server.
  void ConnectedToServer();

  /// \brief This signal is emitted when the client can't send a keep-alive message to the remote server.
  void FailedToSendKeepAliveMessage();

  /// \brief This signal is emmitted when this client thinks it has not heard from the server in a while.
  void NoIncommingData();

  /// \brief This signal is emitted when a new message is received from the remote server.
  void MessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg);

  /// \brief This signal is emmitted when a message has been successfully sent.
  /// The start and end times indicate the time it took to process the Send() request.
  void MessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

private slots:

  /// See QThread manual page, this slot is used to launch processing in another worker thread.
  void OnStartProcessing();

  // To process signals from NiftyLinkClientProcess, which are just propagated.
  void OnProcessesConnectedToRemote();
  void OnMessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg);
  void OnMessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);
  void OnCantReachRemote();
  void OnNoResponseFromRemote();

private:

  NiftyLinkQThread       *m_Thread;
  NiftyLinkClientProcess *m_ClientProcess;
};

} // end namespace niftk

#endif
