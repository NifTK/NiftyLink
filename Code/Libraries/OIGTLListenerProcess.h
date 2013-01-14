/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef OIGTLLISTENERPROCESS_H
#define OIGTLLISTENERPROCESS_H

#include "OIGTLProcessBase.h"

//OpenIGTLink includes
#include "igtlBindMessage.h"
#include "igtlPointMessage.h"
#include "igtlStringMessage.h"
#include "igtlImageMessage.h"

class OIGTLListenerProcessTester;

/**
 * \class OIGTLListenerProcess
 * \brief Class for listening on a given local port to receive messages from a
 * remote host via OpenIGTLink socket in a separate Process.
 * 
 * OIGTLListenerProcess implements the fuctionality required to listen on a selected
 * local port and to receive messages. It can be initialized by specifying the local
 * port or by passing a previously created socket pointer. After initialization the
 * Process starts listening on the given port / socket waiting for clients to connect.
 * If a connection is detected the Process attempts to receive messages from the remote
 * client. The received data is processed and interpreted, resulting in an OIGTLMessage, \
 * which is passed on using a Qt signal.
 */
class NIFTYLINKCOMMON_WINEXPORT OIGTLListenerProcess : public OIGTLProcessBase
{
  Q_OBJECT

  friend class OIGTLSocketObject;

signals:

  /// \brief This signal is emitted when a client connects to the local server.
  void clientConnected(void);

  /// \brief This signal is emitted when a client disconnects from the local server.
  void clientDisconnected(bool onPort);

  /// \brief This signal is emitted when a new message is received from the remote peer.
  void messageReceived(OIGTLMessage::Pointer msg);

protected:

   /// \brief Constructor which initialises the class specific member variables
  OIGTLListenerProcess(QObject *parent = 0);

  /// \brief A simple destructor which ensures that smartpointers will be deleted.
  ~OIGTLListenerProcess(void);

  /// \brief Initialize the Process on a given, externally created OpenIGTLink socket (igtl::Socket), while specifying the related port.
  virtual bool initialize(igtl::Socket::Pointer socket = 0, int port = -1);

  /// \brief Initialize the Process by specifying the local port number to listen on.
  bool initialize(int port);

  /// \brief Function for setting the delay (in msec) between to "Listen" events on the local server socket. 
  ///
  /// This is to detect the connection of a client and to create a specific socket handler.
  virtual void setListenInterval(int msec);

  /// \brief Returns the currently applied listen interval in msecs.
  virtual  int getListenInterval(void);

protected slots:

  /// \brief This slot catches the start request signal, checks if the Process was initialized, and starts the processing if it was.
  virtual void startProcess();

  /// \brief This slot catches the stop request signal, and stops the process (m_running = false)
  virtual void stopProcess();

  /// \brief This function does the cleaning up after the process has stopped.
  virtual void terminateProcess();

  /// \brief This function decides whether the Process is listening on a port or an external socket, and calls the appropriate member function: listenOnPort() or listenOnSocket().
  virtual void doProcessing(void);

protected:

private:
  /// \brief Attempt to activate the Process: do an overall sanity check to see if all required objects / parameters are correctly initialized.
  virtual bool activate(void);

  /** \brief When listening on a port first a ServerSocket is created which waits for clients to connect, then on connection the receive loop starts
  * which attempts to read data from the socket repeatedly.
  */
  void listenOnPort(void);

  /// \brief Executes the listening loop on the given socket waiting for messages to arrive by repeatadly calling receiveMessage().
  void listenOnSocket(void);
  
  /** \brief This function receives and interprets the data if any arrived, and wraps it into the appropriate OIGTLMessage format.
  * The function return true if a valid message was received, else return fals.
  */
  bool receiveMessage(void);

private slots:

  /// \brief This slot is triggered when the socket timeouts: eg there was no activity for the set period of time (m_socketTimeout)
  virtual void socketTimeout(void);
  //void debugTimeoutSignal() { qDebug() <<"Timer timout signal received "; }

private:
  igtl::ServerSocket::Pointer  m_serverSocket;
  bool m_listeningOnPort;
  bool m_clientConnected;
  bool m_timeouterInitialized;
  int m_listenInterval;
};

/**
 * \class OIGTLListenerProcessTester
 * \brief Class derived from OIGTLListenerProcess to enable unit testing.
 */
class NIFTYLINKCOMMON_WINEXPORT OIGTLListenerProcessTester : public OIGTLListenerProcess
{
  Q_OBJECT

  friend class TestListenerProcess_General;

  OIGTLListenerProcessTester(QObject *parent = 0)
    : OIGTLListenerProcess(parent) {}
};


#endif
