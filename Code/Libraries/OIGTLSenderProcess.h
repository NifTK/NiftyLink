/*=============================================================================

 NiftyLink:  A software library to facilitate communication over OpenIGTLink.

             http://cmic.cs.ucl.ac.uk/
             http://www.ucl.ac.uk/

 Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

 Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
 Revision          : $Revision: 3300 $
 Last modified by  : $Author: mjc $

 Original author   : m.clarkson@ucl.ac.uk

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 ============================================================================*/

#ifndef OIGTLSENDERPROCESS_H
#define OIGTLSENDERPROCESS_H

#include "OIGTLProcessBase.h"

class OIGTLSenderProcessTester;

/**
 * \class OIGTLSenderProcess
 * \brief Class for sending messages to a remote host via OpenIGTLink socket in a separate Process.
 * 
 * OIGTLSenderProcess implements the fuctionality required to establish connection to a remote host and to transmit messages. 
 * It can be initialized with a combination of hostname and port or on a previously created socket pointer.
 * When initilized using hostname + port the Process will attempt to connect to the remote host and then enter an idle loop waiting for messages.
 * Data to be sent to the remote host are received via a QT slot in the form of an OIGTLMessage. These messages are appended to the end of a message queue (FIFO).
 */

class NIFTYLINKCOMMON_WINEXPORT OIGTLSenderProcess : public OIGTLProcessBase
{
  Q_OBJECT

  friend class OIGTLSocketObject;

signals:
  /// \brief This signal is emitted when successfully connected to a remote host.
  void connectedToRemote(void);
  /// \brief This signal is emitted when the connection to the remote host cannot be established.
  void cannotConnectToRemote();
  /// \brief This signal is emitted when the client gets disconnected from the local host.
  void disconnectedFromRemote(bool onPort);
  /// \brief This signal is emitted when the client has sent all messages from the message queue.
  void sendingFinished(void);
  /// \brief This signal is emitted when the client has sent all messages from the message queue.
  void messageSent(unsigned long long timestamp);

protected:
  /// \brief Constructor which initialises the class specific member variables
  OIGTLSenderProcess(QObject *parent = 0);
  /// \brief A simple destructor which ensures that smartpointers will be deleted.
  ~OIGTLSenderProcess(void);

  /// \brief Initialize the Process on a given, externally created OpenIGTLink socket (igtl::Socket), while specifying the related port.
  virtual bool initialize(igtl::Socket::Pointer socket = 0, int port = -1);
  /// \brief Initialize the Process by specifying a hostname and port number.
  bool initialize(std::string &hostname, int port = 0);

  /// \brief This function is for setting the timeout interval (in seconds) when connecting to a remote host.
  virtual void setConnectTimeOut(int sec);
  /// \brief It returns the currently applied connection timeout in seconds.
  virtual  int getConnectTimeOut(void);

protected slots:
  /// \brief This slot catches the Qt signal with the messages to be sent, appends the message to the message queue and starts the Process by calling start().
  virtual void sendMsg(OIGTLMessage::Pointer);

  /// \brief This slot catches the start request signal, checks if the Process was initialized, and it starts the main processing loop if it was.
  virtual void startProcess();
  /// \brief This slot catches the stop request signal, and stops the process (m_running = false)
  virtual void stopProcess();

  /// \brief This function does the cleaning up after the process has stopped.
  virtual void terminateProcess();

  /** \brief Main Process functionality takes place in this function.
  *  
  * The newly started Process will first attempt to connect to the specified remote host, unless it has been initialized on an external socket, or already connected to remote.
  * Then the sender loop starts, which is processing the message queue and transmitting messages to the remote host, or idles if the queue is empty.
  * The sender loop continues until the Process is stopped or when the remote host terminates the socket.
  */
  virtual void doProcessing(void);

private:
  /// \brief Attempt to activate the Process: do an overall sanity check to see if all required objects / parameters are correctly initialized.
  virtual bool activate(void);

private slots:
  //void keepaliveTimeout(void);

private:
  igtl::ClientSocket::Pointer m_clientSocket;
  std::string m_hostname;
  bool m_sendingOnSocket;
  int  m_connectTimeout;

  QList<OIGTLMessage::Pointer> m_sendQue;
  QMutex m_queueMutex;
};

class NIFTYLINKCOMMON_WINEXPORT OIGTLSenderProcessTester : public OIGTLSenderProcess
{
  Q_OBJECT

  friend class TestSenderProcess_General;

  OIGTLSenderProcessTester(QObject *parent = 0)
    : OIGTLSenderProcess(parent) {}
};

#endif