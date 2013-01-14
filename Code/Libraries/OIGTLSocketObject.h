/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef OIGTLSOCKETOBJECT_H
#define OIGTLSOCKETOBJECT_H

#include "OIGTLSenderProcess.h"
#include "OIGTLListenerProcess.h"
#include "OIGTLProcessBase.h"

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
 * \class OIGTLSocketObject
 * \brief Class for two way communcation using the OpenIGTLink 2.0 protocol 
 * 
 * OIGTLSocketObject implements the fuctionality required to send and receive messages using
 * the OpenIGTLink 2.0 protocol, in a multi-threaded manner. Each OIGTLSocketObject contains
 * a sender and a receiver thread which share a common mutex. A socket object can be requested
 * to connect to a remote host, or to act as a server by listening on a local port. The current
 * connection status is reported by emitting Qt signals. When connected, messages can be sent
 * through the socket and a signal is emitted (messageReceived(..)) when new data has arrived
 * from the remote peer. Connection status can also be queried via member functions
 * (isListening(), isConnected()... ).
 */
class NIFTYLINKCOMMON_WINEXPORT OIGTLSocketObject : public QObject
{
  Q_OBJECT

signals:

  /// \brief This signal is emitted when new data has arrived from the remote peer, caught by the listener thread.
  void messageReceived(OIGTLMessage::Pointer);

  /// \brief This signal is emitted when a message needs to be sent through the sender thread.
  void messageToSend(OIGTLMessage::Pointer);

  /// \brief This signal is emitted when all messages were sent by the sender thread (message queue is empty).
  void sendingFinished();

  /// \brief This signal is emitted when the sender finished sending the last message, the parameter is the send timestamp
  void messageSent(unsigned long long timestamp);
    
  // These signals are to update the outside world about the current connection status

  /// \brief This signal is emitted when connection to the remote host has been successfully established.
  void connectedToRemoteSignal(void);

  /// \brief This signal is emitted when connection to the remote host cannot be established.
  void cannotConnectToRemoteSignal(void);

  /// \brief This signal is emitted when connection is lost to the remote host.
  void lostConnectionToRemoteSignal(void);

  /// \brief This signal is emitted when a client has connected to the local server (listener thread).
  void clientConnectedSignal(void);

  /// \brief This signal is emitted when a client has disconnected from the local server (listener thread).
  void clientDisconnectedSignal(void); 

  /// \brief This signal is emmitted when the closeSocket method has successfully closed down the listener thread.
  void shutdownListener();

  /// \brief This signal is emmitted when the closeSocket method has successfully closed down the sender thread.
  void shutdownSender();

public:

  /// \brief Constructor which initialises member variables and calls initThreads()
  OIGTLSocketObject(QObject *parent = 0);

  /// \brief A simple destructor which disconnects signals and ensures that they are deleted.
  ~OIGTLSocketObject(void);

  /// \brief This function sets the local listening port and starts the listener thread.
  bool listenOnPort(int port);

  /// \brief This function sets the remote host and port as URL, validates them. If valid, then it pass them on to the sender thread, and starts the thread.
  bool connectToRemote(QUrl url);

  /// \brief Shuts down the socket and sets the flags accordingly.
  void closeSocket(void);

  /// \brief Returns the current listening port
  inline int getPort() { return m_port; }

  /// \brief Returns true if the listener is running
  inline bool isListening() { return m_listening; }

  /// \brief Returns true if the sender is connected to a remote host
  inline bool isConnected() { return m_connectedToRemote; }

  /// \brief Returns true if a client is connected to the listener
  inline bool isClientConnecting() { return m_clientConnected; }

  /// \brief Returns if the socket is valid for sending data
  inline bool isAbleToSend() { return m_ableToSend; }

  /// \brief Returns true if the socket is active (one of the threads are running).
  inline bool isActive() { return m_active; }

  /// \brief Sets the object names, including the sender / listener threads etc.
  void setObjectNames(QString name);

public slots:

  /// \brief This slot catches the signal with the message to send and it pass it on to the sender thread
  void sendMessage(OIGTLMessage::Pointer msg);

  /// \brief This slot captures the signal from the listener thread with the newly arrived data
  void catchMsgSignal(OIGTLMessage::Pointer msg);

  /// \brief Sets the connection timeout of the sender thread (in sec)
  void setConnectTimeOut(int sec);

  /// \brief Returns the currently applied connection timeout (in sec)
   int getConnectTimeOut(void);

  /// \brief Sets the delay between to listen events on the local port
  void setListenInterval(int msec);

  /// \brief Returns the currently applied listen interval of the listener thread
   int getListenInterval(void);

  /// \brief Sets the general timeout limit for both sender and listener threads in msec
  void setSocketTimeOut(int msec);

  /// \brief Returns the currently applied timeout in msec
   int getSocketTimeOut(void);

   /// \brief Initialises the windows tsc timer when required
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
   static void initializeWinTimers();
#endif

private:
  /// \brief Instantiates the sender and listener threads and the shared mutex, and sets up the signal - slot connections
  void initThreads(void);

private slots:

  /// \brief This slot is triggered when the sender connects to a remote host
  void connectedToRemote(void);

  /// \brief This slot is triggered when the connection to a remote host cannot be established
  void cannotConnectToRemote(void);

  /// \brief This slot is triggered when the remote host terminates the connection
  void disconnectedFromRemote(bool onPort);

  /// \brief This slot is triggered when a client connects to the local listener
  void clientConnected(void);

  /// \brief This slot is triggered when a client disconnects from the local server
  void clientDisconnected(bool onPort);

private:

  int      m_port;
  QMutex * m_mutex;

  OIGTLSenderProcess   * m_sender;
  OIGTLListenerProcess * m_listener;

  QThreadEx * m_senderHostThread;
  QThreadEx * m_listenerHostThread;

  bool m_active;
  bool m_initialized;

  bool m_listening;
  bool m_connectedToRemote;

  bool m_clientConnected;
  bool m_ableToSend;

};

#endif
