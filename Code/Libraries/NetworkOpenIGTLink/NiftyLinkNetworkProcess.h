/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkNetworkProcess_h
#define NiftyLinkNetworkProcess_h

#include <NiftyLinkCommonWin32ExportHeader.h>
#include <NiftyLinkMessageContainer.h>
#include <NiftyLinkSocket.h>

#include <QObject>

class QTimer;

namespace niftk
{

/**
* \class NiftyLinkNetworkProcess
* \brief Abstract base class of NiftyLinkServerProcess and NiftyLinkClientProcess.
*
* Note: Error handling strategy is to throw std::exception sub-classes for everything.
*
* This class is designed to work in a separate thread, so effectively the DoProcessing
* virtual method is implemented by subclasses, and logically is equivalent to the
* 'run' method on QThread. So when DoProcessing is finished, the object should
* be discarded, and cannot be re-used.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkNetworkProcess : public QObject {

  Q_OBJECT

public:

  /// \brief Constructor.
  NiftyLinkNetworkProcess(QObject *parent = 0);

  /// \brief Destructor.
  virtual ~NiftyLinkNetworkProcess();

  /// \brief Sets the socket timeout in milliseconds, default = 50 msec.
  void SetSocketTimeout(const int& msec);

  /// \brief Returns the socket timeout in milliseconds.
  int GetSocketTimeout() const;

  /// \brief Starts the main processing loop.
  void StartProcessing();

  /// \brief Stops the main processing loop.
  void StopProcessing();

  /// \brief Sends the OpenIGTLink message (blocking), which should be Unpacked when passed in, as this methods Packs it.
  void Send(igtl::MessageBase::Pointer msg);

signals:

  /// \brief This signal is emitted when a new message is received from the remote peer.
  void MessageReceived(niftk::NiftyLinkMessageContainer::Pointer msg);

  /// \brief This signal is emmitted when a message has been successfully sent.
  /// The start and end times indicate the time it took to process the Send() request.
  void MessageSent(igtlUint64 startTimeInNanoseconds, igtlUint64 endTimeInNanoseconds);

  /// \brief This signal is emmitted when we think the connection is dead (keep-alive failed to send).
  /// Remember that this base class could be for a client or server.
  void CantReachRemote();

  /// \brief This signal is emmitted when we haven't heard from the other end in a while (no data from remote).
  void NoResponseFromRemote();

  /// \brief This internal signal is to start the main processing loop after initialising.
  void InternalStartWorkingSignal(void);

  /// \brief Emmitted to indicate that we have started shutting down.
  void TerminateStarted();

  /// \brief Emmitted to indicate that we have finished shutting down.
  void TerminateFinished();

protected slots:

  /// \brief This is basically the "run" function for when inside another thread.
  virtual void DoProcessing(void) = 0;

  /// \brief Derived classes can decide whether to implement a keep-alive message send.
  virtual void OnKeepAliveTimerTimedOut();

  /// \brief Derived classes must implement a function to decide what to do when no data has been received.
  virtual void OnNoResponseTimerTimedOut();

protected:

  /// \brief Gets the keep alive interval.
  int GetKeepAliveInterval() const;

  /// \brief Sets the keep alive interval.
  void SetKeepAliveInterval(const int& msec);

  /// \brief Starts the keep alive timer.
  void StartKeepAliveTimer();

  /// \brief Stops the keep alive timer.
  void StopKeepAliveTimer();

  /// \brief Gets the no response timer interval.
  int GetNoResponseInterval() const;

  /// \brief Sets the no response timer interval.
  void SetNoResponseInterval(const int& msec);

  /// \brief Starts the no response timer.
  void StartNoResponseTimer();

  /// \brief Stops the no response timer.
  void StopNoResponseTimer();

  /// \brief Returns whether or not sub-classes should continue processing stuff.
  bool GetIsRunning() const;

  /// \brief Continuously loops round receiving incoming messages.
  void ReceiveMessageLoop();

  /// \brief Processes an incomming message.
  bool ReceiveMessage();

  /// \brief Method thats triggered to extract some statistics of latencies.
  void DumpStats();

  /// \brief Sends a message to the remote to dump stats.
  void RequestRemoteStats();

  // Simple running counters of all messages received.
  igtlUint64                     m_NumberOfMessagesReceived;
  igtlUint64                     m_NumberOfMessagesSent;

  // Tracks whether this object thinks he is fully connected.
  bool                           m_IsConnected;

  // This socket can be either a client or server, created at initialisation time.
  NiftyLinkSocket::Pointer       m_MySocket;

  // For a server, this is the allocated single client socket, when the client establishes communications.
  // For the client, this always == m_MySocket.
  NiftyLinkSocket::Pointer       m_CommsSocket;

private slots:

  /// \brief Runs the processing loop, by calling DoProcessing() which derived classes actually implement.
  /// So, here we are using 'Template Method' pattern. Don't change this method, and don't
  /// attempt to make it virtual and override it.
  void RunProcessing();

private:

  /// \brief Shutdown/Destroy stuff, called at the end of processing loop, as processing thread will finish, and from destructor.
  void TerminateProcess();

  /// \brief Contains the system level, standard keep-alive message.
  static const std::string m_KEEP_ALIVE_MESSAGE;

  /// \brief Contains the system level, standard message, to extract stats;
  static const std::string m_STATS_MESSAGE;

  // The timeout in milliseconds passed onto the socket object.
  int                            m_SocketTimeout;

  // For sending keep alive pings.
  QTimer                        *m_KeepAliveTimer;
  int                            m_KeepAliveInterval;

  // For timing how long since we last heard from the other end.
  QTimer                        *m_NoResponseTimer;
  int                            m_NoResponseInterval;

  // Control variable to inform derived classes whether they should be processing or not.
  bool                           m_IsRunning;

  // We track the last time we processed a real message (not a keep-alive), so we can avoid sending keep-alive if not needed.
  igtl::TimeStamp::Pointer       m_LastMessageProcessedTime;

  // This used for calculating stats.
  QList<igtlUint64>              m_ListOfLatencies;
}; // end class

} // end namespace niftk

#endif // NiftyLinkServerProcess_h
