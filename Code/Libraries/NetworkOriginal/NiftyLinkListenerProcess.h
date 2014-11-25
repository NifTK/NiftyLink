/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkListenerProcess_h
#define NiftyLinkListenerProcess_h

#include "NiftyLinkProcessBase.h"

#include "igtlBindMessage.h"
#include "igtlPointMessage.h"
#include "igtlStringMessage.h"
#include "igtlImageMessage.h"

namespace niftk
{

class NiftyLinkListenerProcessTester;

/**
* \class NiftyLinkListenerProcess
* \brief Class for listening on a given local port to receive messages from a
* remote host via OpenIGTLink socket in a separate Process.
*
* NiftyLinkListenerProcess implements the fuctionality required to listen on a selected
* local port and to receive messages. It can be initialized by specifying the local
* port or by passing a previously created socket pointer. After initialization the
* Process starts listening on the given port / socket waiting for clients to connect.
* If a connection is detected the Process attempts to receive messages from the remote
* client. The received data is processed and interpreted, resulting in an NiftyLinkMessage, \
* which is passed on using a Qt signal.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkListenerProcess : public NiftyLinkProcessBase
{
  Q_OBJECT

  friend class NiftyLinkSocketObject;

signals:

  /// \brief This signal is emitted when a client connects to the local server.
  void ClientConnectedSignal(void);

  /// \brief This signal is emitted when a client disconnects from the local server.
  void ClientDisconnectedSignal(bool onPort);

  /// \brief This signal is emitted when a new message is received from the remote peer.
  void MessageReceivedSignal(NiftyLinkMessage::Pointer msg);

protected:

  /// \brief Constructor which initialises the class specific member variables.
  NiftyLinkListenerProcess(QObject *parent = 0);

  /// \brief A simple destructor which ensures that smartpointers will be deleted.
  virtual ~NiftyLinkListenerProcess(void);

  /// \brief Initialize the process on a given externally created OpenIGTLink socket (igtl::Socket), while specifying the related port.
  virtual bool Initialize(niftk::NiftyLinkSocket::Pointer socket = 0, int port = -1);

  /// \brief Initialize the process by specifying the local port number to listen on.
  bool Initialize(int port);

  /// \brief Function for setting the delay (in msec) between to "Listen" events on the local server socket.
  virtual void SetListenInterval(int msec);

  /// \brief Returns the currently applied listen interval in msecs.
  virtual int GetListenInterval(void);

protected slots:

  /// \brief This slot catches the start request signal, checks if the Process was initialized, and starts the processing if it was.
  virtual void StartProcess();

  /// \brief This slot catches the stop request signal and stops the process (m_running = false).
  virtual void StopProcess();

  /// \brief This function does the cleaning up after the process has stopped.
  virtual void TerminateProcess();

  /// \brief This function decides whether the process is listening on a port
  /// or on an external socket and calls the appropriate member function: ListenOnPort() or ListenOnSocket().
  virtual void DoProcessing(void);

protected:

private:
  /// \brief Attempt to activate the Process: do an overall sanity check to see if all required objects and parameters are correctly initialized.
  virtual bool Activate(void);

  /** \brief When listening on a port first a ServerSocket is created which waits for clients to connect, then on connection the receive loop starts
  * which attempts to read data from the socket repeatedly.
  */
  void ListenOnPort(void);

  /// \brief Executes the listening loop on the given socket waiting for messages to arrive by repeatadly calling ReceiveMessage().
  void ListenOnSocket(void);

  /** \brief This function receives and interprets the data if any arrived, and wraps it into the appropriate NiftyLinkMessage format.
  * The function return true if a valid message was received, else return fals.
  */
  bool ReceiveMessage(void);

private slots:

  /// \brief This slot is triggered when the socket timeouts: eg there was no activity for the set period of time (m_socketTimeout)
  virtual void OnSocketTimeout(void);
  //void debugTimeoutSignal() { qDebug() <<"Timer timout signal received "; }

private:
  niftk::NiftyLinkServerSocket::Pointer m_ServerSocket;
  bool                                  m_ListeningOnPort;
  bool                                  m_ClientConnected;
  bool                                  m_TimeOuterInitialized;
  int                                   m_ListenInterval;
};

} // end namespace niftk

#endif // NiftyLinkListenerProcess_h
