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

#ifndef OIGTLPROCESSBASE_H
#define OIGTLPROCESSBASE_H

//Qt related includes
#include <QtCore>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QTimer>

//OpenIGTLink includes
#include "igtlSocket.h"
#include "igtlServerSocket.h"

//NiftyLink includes
#include "OIGTLMessage.h"
#include "OIGTLTransformMessage.h"
#include "OIGTLTrackingDataMessage.h"
#include "OIGTLImageMessage.h"
#include "OIGTLStatusMessage.h"
#include "OIGTLStringMessage.h"
#include "NiftyLinkUtils.h"

//Logging
#include "QsLog.h"
#include "QsLogDest.h"

#include "NiftyLinkCommonWin32ExportHeader.h"

/**
 * \class OIGTLProcessBase
 * \brief Provides the base class for the NiftyLink Sender and Listner Processes.
 * 
 * OIGTLProcessBase is the base class of the communication Processes, which perform two way communication within a NiftyLink socket.
 * It defines the common functionality and common data members for managing the communication Processes.
 */

class OIGTLProcessBase : public QObject
{
  Q_OBJECT

signals:

  /// \brief This signal is emitted when there was some acitivty on the socket, so the timeout timer needs to be reset
  void restartTimer(int msec);

  /// \brief This signal is to stop the timeout timer (used on shutdown)
  void stopTimer(void);

  /// \brief This internal signal is to start the main processing loop after startProcess() finished initializing
  void startWorking(void);

  /// \brief This signal is emitted when the process has fully terminated, so it is OK to terminate the hosting thread as well
  void shutdownHostThread(void);

public:

  /// \brief Constructor which initialises the common member variables
  OIGTLProcessBase(QObject *parent = 0);
  
  /// \brief Basic destructor
  ~OIGTLProcessBase(void);

protected:

  /// \brief Initialize the Process on a given, externally created OpenIGTLink socket (igtl::Socket), while specifying the related port. Process specific functionalty are defined in the derived classes.
  virtual bool initialize(igtl::Socket::Pointer socket = 0, int port = -1) { return false; }

  /// \brief Attempt to activate the Process: do an overall sanity check to see if all required objects / parameters are correctly initialized. Process specific functionalty are defined in the derived classes.
  virtual bool activate(void) { return false; }
  
  /// \brief Function base to send a message. Only implemented in OIGTLSenderProcess.
  virtual void sendMsg(OIGTLMessage::Pointer) {}

  /// \brief This function returns the actual socket pointer. This is necessary in order to set up a two communication chanel: to initiate a sender Process on a socket created by a listener Process and vica versa.
  virtual igtl::Socket::Pointer getSocketPointer(void) { return m_extSocket; }

  /// \brief This method sets the mutex, this way enables two independent Process (e.g. sender / listener operating on the same socket) to use the same mutex object.
  inline  void setMutex(QMutex * mutex) { m_mutex = mutex; }

  /// \brief Function base for setting connection timeout in seconds when connecting to a remote peer. Only implemented in OIGTLSenderProcess.
  virtual void setConnectTimeOut(int sec) {}

  /// \brief Function base getting the currently applied connection timeout in seconds. Only implemented in OIGTLSenderProcess.
  virtual  int getConnectTimeOut(void) { return -1; }

  /// \brief Function base for setting the delay between to "Listen" events on the local server socket. 
  /// This is to detect the connection of a client and to create a specific socket handler. The timeout is in msec. Only implemented in OIGTLListenerProcess. 
  virtual void setListenInterval(int msec) {}

  /// \brief Function base for getting the currently applied listen interval.
  virtual  int getListenInterval(void) { return -1; }

  /// \brief This function sets the general timeout interval of all operations on the socket, in msec.
  inline void setSocketTimeOut(int msec) { m_socketTimeout = msec; }

  /// \brief This function returns the general timeout interval currently applied to the socket, in msec.
  inline  int getSocketTimeOut(void)     { return m_socketTimeout; }

  /// \brief This functions tells if the current is in the state of execution or idle.
  inline  bool isActive() { return m_active; }

  /// \brief This functions tells if the current Process is currently initialized or not.
  inline  bool isInitialized() {return m_initialized; }

  /// \brief This functions returns the current port number associated with the socket.
  inline  int  getPort(void) {return m_port; }
  
protected slots:

  /// \brief Function base for starting a Process. Process specific functionalty are defined in the derived classes.
  virtual void startProcess() = 0;
  
  /// \brief Function base for stopping a Process. Process specific functionalty are defined in the derived classes.
  virtual void stopProcess() = 0;

  /// \brief Function base for cleaning up after the process has stopped. Process specific functionalty are defined in the derived classes.
  virtual void terminateProcess() = 0;

  /// \brief This slot is triggered when the socket timeouts: eg there was no activity for the set period of time (m_socketTimeout)
  virtual void socketTimeout(void) {}

  /// \brief This is the "run" function base, which implements the required functionality of the Process in the derived classes.
  virtual void doProcessing(void) = 0;

protected:
  int      m_port;
  int      m_socketTimeout;

  bool     m_running;   //this bool is to control the running state of the process
  bool     m_active;    //this bool is to indicate to the outside world if the process is running or not
                        //m_active is false by default, true when the process is running, and gets false again only if the process has fully stopped

  bool     m_initialized;
  QMutex * m_mutex;
  QTimer * m_timeouter;

  unsigned long m_messageCounter;

  igtl::Socket::Pointer m_extSocket;
};

/**
 * \class QThreadEx
 * \brief Extends QThread and makes it sure that the event loop has started.
 * 
 * QThreadEx extends the functionality of QThread. In the default implementation of QThread there is no way to tell if the thread's event loop
 * is running or not. Therefore we subclass QThread, and add custom signals and functions to check for this. Also, we expose the thread's 
 * sleep function via member function.
 *
 */

class  NIFTYLINKCOMMON_WINEXPORT QThreadEx : public QThread
{
  Q_OBJECT 

signals:
  void eventloopStarted(void);

private:
  bool m_isEventloopRunning;

public:
  /// \brief Simple constructor
  inline QThreadEx() : QThread() { m_isEventloopRunning = false; }

  /// \brief Member function to put the thread into sleep
  inline void msleepEx(int msec) { this->msleep(msec); }
  
  /// \brief Member function to check if the eventloop has started
  inline bool isEventloopRunning(void) { return m_isEventloopRunning; }

private slots:

  /// \brief A slot to capture the internal signal after the eventloop has started
  void catchSignal(void)
  { 
    //qDebug() <<"Caught the timer's signal"; 
    m_isEventloopRunning = true;
    
    emit eventloopStarted(); 
  }

protected:

  /// \brief Extends the default QThread::run() function by firing a signal with a QTimer, which is getting delivered after the event loop has started
  virtual void run() 
  { 
    //qDebug() <<"Calling exec on thread" ;
    QTimer::singleShot(0, this, SLOT(catchSignal()));
    QCoreApplication::processEvents();
    exec(); 
  }
}; 

#endif
