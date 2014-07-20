/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkProcessBase_h
#define NiftyLinkProcessBase_h

#include "NiftyLinkCommonWin32ExportHeader.h"

#include <NiftyLinkSocket.h>
#include <NiftyLinkServerSocket.h>
#include <NiftyLinkMessage.h>
#include <NiftyLinkTransformMessage.h>
#include <NiftyLinkTrackingDataMessage.h>
#include <NiftyLinkImageMessage.h>
#include <NiftyLinkStatusMessage.h>
#include <NiftyLinkStringMessage.h>
#include <NiftyLinkUtils.h>

#include <QtCore>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QTimer>

#include <QsLog.h>
#include <QsLogDest.h>

/**
* \class NiftyLinkProcessBase
* \brief Provides the base class for the NiftyLink Sender and Listner Processes.
*
* NiftyLinkProcessBase is the base class of the communication Processes, which perform two way communication within a NiftyLink socket.
* It defines the common functionality and common data members for managing the communication Processes.
*/

class NiftyLinkProcessBase : public QObject
{
  Q_OBJECT

signals:

  /// \brief This signal is emitted when there was some acitivty on the socket, so the timeout timer needs to be reset
  void RestartTimerSignal(int msec);

  /// \brief This signal is to stop the timeout timer (used on shutdown)
  void StopTimerSignal(void);

  /// \brief This internal signal is to start the main processing loop after startProcess() finished initializing
  void StartWorkingSignal(void);

  /// \brief This signal is emitted when the process has fully terminated, so it is OK to terminate the hosting thread as well
  void ShutdownHostThreadSignal(void);

  /// \brief This internal signal is to pass on the message access times, that is useful for debugging transmission lag
  void SendMessageAccessTimes(QString);

public:

  /// \brief Constructor which initialises the common member variables
  NiftyLinkProcessBase(QObject *parent = 0);

  /// \brief Basic destructor
  virtual ~NiftyLinkProcessBase(void);

protected:

  /// \brief Initialize the Process on a given, externally created OpenIGTLink socket (igtl::Socket),
  /// while specifying the related port. Process specific functionalty are defined in the derived classes.
  virtual bool Initialize(NiftyLinkSocket::Pointer socket = 0, int port = -1)
  {
    return false;
  }

  /// \brief Attempt to activate the Process: do an overall sanity check to see if all required objects
  /// and parameters are correctly initialized. Process specific functionalty are defined in the derived classes.
  virtual bool Activate(void)
  {
    return false;
  }

  /// \brief Function base to add a message to the send queue. Only implemented in NiftyLinkSenderProcess.
  virtual void AddMsgToSendQueue(NiftyLinkMessage::Pointer) {}

  /// \brief This function returns the actual socket pointer. This is necessary in order to set up a two communication chanel:
  /// to initiate a sender Process on a socket created by a listener Process and vica versa.
  virtual NiftyLinkSocket::Pointer GetSocketPointer(void)
  {
    return m_ExtSocket;
  }

  /// \brief This method sets the mutex, this way enables two independent Process
  /// (e.g. sender / listener operating on the same socket) to use the same mutex object.
  inline void SetMutex(QMutex * mutex)
  {
    m_Mutex = mutex;
  }

  /// \brief Function base for setting connection timeout in seconds when connecting to a remote peer.
  /// Only implemented in NiftyLinkSenderProcess.
  virtual void SetConnectTimeOut(int sec) {}

  /// \brief Function base getting the currently applied connection timeout in seconds. Only implemented in NiftyLinkSenderProcess.
  virtual int GetConnectTimeOut(void)
  {
    return -1;
  }

  /// \brief Function base for setting the delay between to "Listen" events on the local server socket.
  /// This is to detect the connection of a client and to create a specific socket handler. The timeout is in msec.
  /// Only implemented in NiftyLinkListenerProcess.
  virtual void SetListenInterval(int msec) {}

  /// \brief Function base for getting the currently applied listen interval.
  virtual int GetListenInterval(void)
  {
    return -1;
  }

  /// \brief This function sets the general timeout interval of all operations on the socket, in msec.
  inline void SetSocketTimeOut(int msec)
  {
    m_SocketTimeout = msec;
  }

  /// \brief This function returns the general timeout interval currently applied to the socket, in msec.
  inline int GetSocketTimeOut(void)
  {
    return m_SocketTimeout;
  }

  /// \brief This functions tells if the current is in the state of execution or idle.
  inline bool IsActive()
  {
    return m_Active;
  }

  /// \brief This functions tells if the current Process is currently initialized or not.
  inline bool IsInitialized()
  {
    return m_Initialized;
  }

  /// \brief This functions returns the current port number associated with the socket.
  inline int GetPort(void)
  {
    return m_Port;
  }

protected slots:

  /// \brief Function base for starting a Process. Process specific functionalty are defined in the derived classes.
  virtual void StartProcess() = 0;

  /// \brief Function base for stopping a Process. Process specific functionalty are defined in the derived classes.
  virtual void StopProcess() = 0;

  /// \brief Function base for cleaning up after the process has stopped. Process specific functionalty are defined in the derived classes.
  virtual void TerminateProcess() = 0;

  /// \brief This slot is triggered when the socket timeouts: eg there was no activity for the set period of time (m_socketTimeout)
  virtual void SocketTimeout(void) {}

  /// \brief This is the "run" function base, which implements the required functionality of the Process in the derived classes.
  virtual void DoProcessing(void) = 0;

protected:
  int                       m_Port;
  int                       m_SocketTimeout;

  //this bool is to control the running state of the process
  bool                      m_Running;

  //this bool is to indicate to the outside world if the process is running or not
  //m_active is false by default, true when the process is running, and gets false again only if the process has fully stopped
  bool                      m_Active;

  bool                      m_Initialized;
  QMutex                   *m_Mutex;
  QTimer                   *m_TimeOuter;

  unsigned long             m_MessageCounter;
  NiftyLinkSocket::Pointer  m_ExtSocket;
};

#endif // NiftyLinkProcessBase_h
