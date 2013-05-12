/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkSocketObject_h
#define NiftyLinkSocketObject_h

#include "NiftyLinkSenderProcess.h"
#include "NiftyLinkListenerProcess.h"
#include "NiftyLinkProcessBase.h"

#include "NiftyLinkCommonWin32ExportHeader.h"

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include <bcrypt.h>
#include "tsctime/TSCtime.h"

#define WINAPI __stdcall

// Function pointers that will be used for the DLL functions.
typedef NTSTATUS (__stdcall *FunctionPtr_SETRES) (ULONG, BOOLEAN, PULONG);
typedef NTSTATUS (__stdcall *FunctionPtr_GETRES) (PULONG, PULONG, PULONG);
#endif

/**
* \class NiftyLinkSocketObject
* \brief Class for two way communcation using the OpenIGTLink 2.0 protocol
*
* NiftyLinkSocketObject implements the fuctionality required to send and receive messages using
* the OpenIGTLink 2.0 protocol, in a multi-threaded manner. Each NiftyLinkSocketObject contains
* a sender and a receiver thread which share a common mutex. A socket object can be requested
* to connect to a remote host, or to act as a server by listening on a local port. The current
* connection status is reported by emitting Qt signals. When connected, messages can be sent
* through the socket and a signal is emitted (MessageReceived(..)) when new data has arrived
* from the remote peer. Connection status can also be queried via member functions
* (IsListening(), IsConnected()... ).
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkSocketObject : public QObject
{
  Q_OBJECT

signals:

  /// \brief This signal is emitted when new data has arrived from the remote peer, caught by the listener thread.
  void MessageReceivedSignal(NiftyLinkMessage::Pointer);

  /// \brief This signal is emitted when a message needs to be sent through the sender thread.
  void MessageToSendSignal(NiftyLinkMessage::Pointer);

  /// \brief This signal is emitted when all messages were sent by the sender thread (message queue is empty).
  void SendingFinishedSignal();

  /// \brief This signal is emitted when the sender finished sending the last message, the parameter is the send timestamp
  void MessageSentSignal(unsigned long long timestamp);

  // These signals are to Update the outside world about the current connection status

  /// \brief This signal is emitted when connection to the remote host has been successfully established.
  void ConnectedToRemoteSignal(void);

  /// \brief This signal is emitted when connection to the remote host cannot be established.
  void CannotConnectToRemoteSignal(void);

  /// \brief This signal is emitted when connection is lost to the remote host.
  void LostConnectionToRemoteSignal(void);

  /// \brief This signal is emitted when a client has connected to the local server (listener thread).
  void ClientConnectedSignal(void);

  /// \brief This signal is emitted when a client has dIsConnected from the local server (listener thread).
  void ClientDisconnectedSignal(void);

  /// \brief This signal is emmitted when the CloseSocket method has successfully closed down the listener thread.
  void ShutdownListenerSignal();

  /// \brief This signal is emmitted when the CloseSocket method has successfully closed down the sender thread.
  void ShutdownSenderSignal();

public:

  /// \brief Constructor which initialises member variables and calls InitThreads()
  NiftyLinkSocketObject(QObject *parent = 0);

  /// \brief A simple destructor which disconnects signals and ensures that they are deleted.
  virtual ~NiftyLinkSocketObject(void);

  /// \brief This function sets the local listening port and starts the listener thread.
  bool ListenOnPort(int port);

  /// \brief This function sets the remote host and port as URL, validates them. If valid, then it pass them on to the sender thread, and starts the thread.
  bool ConnectToRemote(QUrl url);

  /// \brief Shuts down the socket and sets the flags accordingly.
  void CloseSocket(void);

  /// \brief Returns the current listening port
  inline int GetPort()
  {
    return m_Port;
  }

  /// \brief Returns true if the listener is running
  inline bool IsListening()
  {
    return m_Listening;
  }

  /// \brief Returns true if the sender is connected to a remote host
  inline bool IsConnected()
  {
    return m_ConnectedToRemote;
  }

  /// \brief Returns true if a client is connected to the listener
  inline bool IsClientConnecting()
  {
    return m_ClientConnected;
  }

  /// \brief Returns if the socket is valid for sending data
  inline bool IsAbleToSend()
  {
    return m_AbleToSend;
  }

  /// \brief Returns true if the socket is active (one of the threads are running).
  inline bool IsActive()
  {
    return m_Active;
  }

  /// \brief Sets the object names, including the sender / listener threads etc.
  void SetObjectNames(QString name);

public slots:

  /// \brief This slot catches the signal with the message to send and it pass it on to the sender thread
  void SendMessage(NiftyLinkMessage::Pointer msg);

  /// \brief This slot captures the signal from the listener thread with the newly arrived data
  void CatchMsgSignal(NiftyLinkMessage::Pointer msg);

  /// \brief Sets the connection timeout of the sender thread (in sec)
  void SetConnectTimeOut(int sec);

  /// \brief Returns the currently applied connection timeout (in sec)
  int GetConnectTimeOut(void);

  /// \brief Sets the delay between to listen events on the local port
  void SetListenInterval(int msec);

  /// \brief Returns the currently applied listen interval of the listener thread
  int GetListenInterval(void);

  /// \brief Sets the general timeout limit for both sender and listener threads in msec
  void SetSocketTimeOut(int msec);

  /// \brief Returns the currently applied timeout in msec
  int GetSocketTimeOut(void);

  /// \brief Initialises the windows tsc timer when required
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
  static void InitializeWinTimers();
#endif

private:
  /// \brief Instantiates the sender and listener threads and the shared mutex, and sets up the signal - slot connections
  void InitThreads(void);

private slots:

  /// \brief This slot is triggered when the sender connects to a remote host
  void OnConnectedToRemote(void);

  /// \brief This slot is triggered when the connection to a remote host cannot be established
  void OnCannotConnectToRemote(void);

  /// \brief This slot is triggered when the remote host terminates the connection
  void OnDisconnectedFromRemote(bool onPort);

  /// \brief This slot is triggered when a client connects to the local listener
  void OnClientConnected(void);

  /// \brief This slot is triggered when a client disconnects from the local server
  void OnClientDisconnected(bool onPort);

private:

  int                    m_Port;
  QMutex               * m_Mutex;

  NiftyLinkSenderProcess   * m_Sender;
  NiftyLinkListenerProcess * m_Listener;

  QThreadEx            * m_SenderHostThread;
  QThreadEx            * m_ListenerHostThread;

  bool                   m_Active;
  bool                   m_Initialized;

  bool                   m_Listening;
  bool                   m_ConnectedToRemote;

  bool                   m_ClientConnected;
  bool                   m_AbleToSend;

};

#endif // NiftyLinkSocketObject_h
