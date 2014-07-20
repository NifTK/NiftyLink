/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkQThread_h
#define NiftyLinkQThread_h

#include "NiftyLinkCommonWin32ExportHeader.h"

#include <QThread>
#include <QTimer>
#include <QCoreApplication>

namespace niftk
{

/**
* \class NiftyLinkQThread
* \brief Extends QThread and makes it sure that the event loop has started.
*
* NiftyLinkQThread extends the functionality of QThread. In the default
* implementation of QThread there is no way to tell if the thread's event loop
* is running or not. Therefore we subclass QThread, and add custom signals and
* functions to check for this. Also, we expose the thread's sleep function
* via member function.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkQThread : public QThread
{
  Q_OBJECT

public:

  /// \brief Simple constructor.
  inline NiftyLinkQThread() : QThread()
  , m_IsEventLoopRunning(false)
  {
  }

  /// \brief Member function to put the thread into sleep.
  inline void MsleepEx(int msec)
  {
    this->msleep(msec);
  }

  /// \brief Member function to check if the eventloop has started.
  inline bool IsEventLoopRunning(void) const
  {
    return m_IsEventLoopRunning;
  }

signals:

  void EventLoopStarted(void);

protected:

  /// \brief Extends the default QThread::run() function by firing
  /// a signal with a QTimer, which is getting delivered after the
  /// event loop has started.
  virtual void run()
  {
    QTimer::singleShot(0, this, SLOT(CatchSignal()));
    QCoreApplication::processEvents();
    exec();
  }

private slots:

  /// \brief A slot to capture the internal signal after the eventloop has started.
  void CatchSignal(void)
  {
    m_IsEventLoopRunning = true;
    emit EventLoopStarted();
  }

private:

  bool m_IsEventLoopRunning;

};

} // end namespace niftk

#endif // NiftyLinkQThread_h
