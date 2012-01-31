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

#ifndef OIGTLTHREADBASE_H
#define OIGTLTHREADBASE_H

//Qt related includes
#include <QtCore>
#include <QObject>
#include <QDebug>
#include <QMutex>

//OpenIGTLink includes
#include "igtlSocket.h"
#include "igtlServerSocket.h"

//NiftyLink includes
#include "OIGTLMessage.h"
#include "OIGTLTransformMessage.h"
#include "NiftyLinkUtils.h"

//Logging
#include "QsLog.h"
#include "QsLogDest.h"


/**
 * \class OIGTLThreadBase
 * \brief Provides the base class for the NiftyLink Sender and Listner threads.
 * 
 * OIGTLThreadBase is the base class of the communication threads, which perform two way communication within a NiftyLink socket.
 * It defines the common functionality and common data members for managing the communication threads by extending QThread's functionality.
 */

class OIGTLThreadBase : public QThread
{
  Q_OBJECT

public:

  /// \brief Constructor which initialises the common member variables
  OIGTLThreadBase(QObject *parent = 0);
  
  /// \brief Basic destructor
  ~OIGTLThreadBase(void);

protected:
  /// \brief This is the "run" function base, which implements the required functionality of the thread in the derived classes.
  virtual void run(void) {}

  /// \brief Initialize the thread on a given, externally created OpenIGTLink socket (igtl::Socket), while specifying the related port. Thread specific functionalty are defined in the derived classes.
  virtual bool initialize(igtl::Socket::Pointer socket = 0, int port = -1) { return false; }
  /// \brief Attempt to activate the thread: do an overall sanity check to see if all required objects / parameters are correctly initialized. Thread specific functionalty are defined in the derived classes.
  virtual bool activate(void) { return false; }
  
  /// \brief Function base to send a message. Only implemented in OIGTLSenderThread.
  virtual void sendMsg(OIGTLMessage &msg) {}
  /// \brief This function returns the actual socket pointer. This is necessary in order to set up a two communication chanel: to initiate a sender thread on a socket created by a listener thread and vica versa.
  virtual igtl::Socket::Pointer getSocketPointer(void) { return m_extSocket; }

  /// \brief This method sets the mutex, this way enables two independent threads (e.g. sender / listener operating on the same socket) to use the same mutex object.
  inline  void setMutex(QMutex * mutex) { m_mutex = mutex; }

  /// \brief Function base for setting connection timeout in seconds when connecting to a remote peer. Only implemented in OIGTLSenderThread.
  virtual void setConnectTimeOut(int sec) {}
  /// \brief Function base getting the currently applied connection timeout in seconds. Only implemented in OIGTLSenderThread.
  virtual  int getConnectTimeOut(void) { return -1; }

  /// \brief Function base for setting the delay between to "Listen" events on the local server socket. 
  /// This is to detect the connection of a client and to create a specific socket handler. The timeout is in msec. Only implemented in OIGTLListenerThread. 
  virtual void setListenInterval(int msec) {}
  /// \brief Function base for getting the currently applied listen interval.
  virtual  int getListenInterval(void) { return -1; }

  /// \brief This function sets the general timeout interval of all operations on the socket, in msec.
  inline void setSocketTimeOut(int msec) { m_socketTimeout = msec; }
  /// \brief This function returns the general timeout interval currently applied to the socket, in msec.
  inline  int getSocketTimeOut(void)     { return m_socketTimeout; }

  /// \brief This functions tells if the current is in the state of execution or idle.
  inline  bool isActive() {return m_running; }

  
  /// \brief This functions tells if the current thread is currently initialized or not.
  inline  bool isInitialized() {return m_initialized; }
  /// \brief This functions returns the current port number associated with the socket.
  inline  int  getPort(void) {return m_port; }
  

protected slots:
  /// \brief Function base for starting a thread. Thread specific functionalty are defined in the derived classes.
  virtual void startThread() {}
  
  /// \brief Function base for stopping a thread. Thread specific functionalty are defined in the derived classes.
  virtual void stopThread() {}

protected:
  int      m_port;
  int      m_socketTimeout;
  bool     m_running;
  bool     m_initialized;
  QMutex * m_mutex;

  igtl::Socket::Pointer m_extSocket;
};

#endif
