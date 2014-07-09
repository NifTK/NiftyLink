/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkSenderProcess_h
#define NiftyLinkSenderProcess_h

#include "NiftyLinkProcessBase.h"

class NiftyLinkSenderProcessTester;

/**
* \class NiftyLinkSenderProcess
* \brief Class for sending messages to a remote host via OpenIGTLink socket in a separate Process.
*
* NiftyLinkSenderProcess implements the fuctionality required to establish connection to a
* remote host and to transmit messages. It can be initialized with a combination of
* hostname and port or on a previously created socket pointer. When initilized using
* hostname + port the Process will attempt to connect to the remote host and then enter an idle
* loop waiting for messages. Data to be sent to the remote host are received via a QT slot
* in the form of an NiftyLinkMessage. These messages are appended to the end of a message queue (FIFO).
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkSenderProcess : public NiftyLinkProcessBase
{
  Q_OBJECT

  friend class NiftyLinkSocketObject;

signals:

  /// \brief This signal is emitted when successfully connected to a remote host.
  void ConnectedToRemoteSignal(void);

  /// \brief This signal is emitted when the connection to the remote host cannot be established.
  void CannotConnectToRemoteSignal(void);

  /// \brief This signal is emitted when the client gets disconnected from the local host.
  void DisconnectedFromRemoteSignal(bool onPort);

  /// \brief This signal is emitted when the client has sent all messages from the message queue.
  void SendingFinishedSignal(void);

  /// \brief This signal is emitted when the client has sent all messages from the message queue.
  void MessageSentSignal(unsigned long long timestamp);

protected:

  /// \brief Constructor which initialises the class specific member variables
  NiftyLinkSenderProcess(QObject *parent = 0);

  /// \brief A simple destructor which ensures that smartpointers will be deleted.
  virtual ~NiftyLinkSenderProcess(void);

  /// \brief Initialize the Process on a given, externally created OpenIGTLink socket (igtl::Socket), while specifying the related port.
  virtual bool Initialize(igtl::Socket::Pointer socket = 0, int port = -1);

  /// \brief Initialize the Process by specifying a hostname and port number.
  bool Initialize(std::string &hostname, int port = 0);

  /// \brief This function is for setting the timeout interval (in seconds) when connecting to a remote host.
  virtual void SetConnectTimeOut(int sec);

  /// \brief It returns the currently applied connection timeout in seconds.
  virtual int GetConnectTimeOut(void);

protected slots:

  /// \brief This slot catches the Qt signal with the messages to be sent, appends the message to the message queue and starts the Process by calling start().
  virtual void AddMsgToSendQueue(NiftyLinkMessage::Pointer);

  /// \brief This slot catches the start request signal, checks if the Process was initialized, and it starts the main processing loop if it was.
  virtual void StartProcess();

  /// \brief This slot catches the stop request signal, and stops the process (m_running = false)
  virtual void StopProcess();

  /// \brief This function does the cleaning up after the process has stopped.
  virtual void TerminateProcess();

  /** \brief Main Process functionality takes place in this function.
  *
  * The newly started Process will first attempt to connect to the specified remote host, unless it has been initialized on an external socket,
  * or already connected to remote. Then the sender loop starts, which is processing the message queue and transmitting messages to the remote host,
  * or idles if the queue is empty. The sender loop continues until the Process is stopped or when the remote host terminates the socket.
  */
  virtual void DoProcessing(void);

private:

  /// \brief Attempt to activate the Process: do an overall sanity check to see if all required objects / parameters are correctly initialized.
  virtual bool Activate(void);

private slots:
  void OnKeepAliveTimeout(void);

private:
  igtl::ClientSocket::Pointer      m_ClientSocket;
  std::string                      m_Hostname;
  bool                             m_SendingOnSocket;
  int                              m_ConnectTimeout;
  int                              m_KeepAliveTimeout;

  QList<NiftyLinkMessage::Pointer> m_SendQue;
  QMutex                           m_QueueMutex;
};

/**
* \class NiftyLinkSenderProcessTester
* \brief Class derived from NiftyLinkSenderProcess to enable unit testing.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkSenderProcessTester : public NiftyLinkSenderProcess
{
  Q_OBJECT

  friend class TestSenderProcess_General;

  NiftyLinkSenderProcessTester(QObject *parent = 0)
    : NiftyLinkSenderProcess(parent) {}
};

#endif // NiftyLinkSenderProcess_h
