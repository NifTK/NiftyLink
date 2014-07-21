/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkServerProcess.h"
#include <NiftyLinkQThread.h>
#include <NiftyLinkMacro.h>

#include <QsLog.h>

#include <stdexcept>
#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkServerProcess::NiftyLinkServerProcess()
: m_ListenInterval(1000)
{
  this->setObjectName("NiftyLinkServerProcess");
}


//-----------------------------------------------------------------------------
NiftyLinkServerProcess::~NiftyLinkServerProcess()
{
}


//-----------------------------------------------------------------------------
void NiftyLinkServerProcess::SetListenInterval(const int &msec)
{
  m_ListenInterval = msec;
}


//-----------------------------------------------------------------------------
int NiftyLinkServerProcess::GetListenInterval()
{
  return m_ListenInterval;
}


//-----------------------------------------------------------------------------
void NiftyLinkServerProcess::Initialise(const int& port)
{  
  // Set up a new instance of listener on a given port and create a new server socket
  if (port <= 0)
  {
    QString errorMessage = QObject::tr("%1::Initialise(%2) - Cannot create a server socket, invalid port specified.").arg(objectName()).arg(port);
    QLOG_ERROR() << errorMessage;
    NiftyLinkStdExceptionMacro(std::invalid_argument, << errorMessage.toStdString());
  }

  if (m_MySocket.IsNotNull())
  {
    QString errorMessage = QObject::tr("%1::Initialise(%2) - Server already in use, you should not try to re-initialise it.").arg(objectName()).arg(port);
    QLOG_ERROR() << errorMessage;
    NiftyLinkStdExceptionMacro(std::logic_error, << errorMessage.toStdString());
  }

  NiftyLinkServerSocket::Pointer ssock = NiftyLinkServerSocket::New();
  int err = ssock->CreateServer(port);
  if (err < 0)
  {    
    QString errorMessage = QObject::tr("%1::Initialise(%2) - Error creating server socket. Check console/log file.").arg(objectName()).arg(port);
    QLOG_ERROR() << errorMessage;
    NiftyLinkStdExceptionMacro(std::runtime_error, << errorMessage.toStdString());
  }

  ssock->SetTimeout(this->GetSocketTimeout());
  m_MySocket = ssock;
  m_IsConnected = false; // this gets set when we believe we have full connection set up (i.e. both ends).

  // For logging. We do this once we have initialised, as by now the port is deemed to be valid.
  this->setObjectName(QObject::tr("NiftyLinkServerProcess(%1)").arg(port));

  QLOG_INFO() << QObject::tr("%1::Initialise(%2) - successfully initialised.").arg(objectName()).arg(port);
}


//-----------------------------------------------------------------------------
void NiftyLinkServerProcess::DoProcessing()
{
  // We MUST be running within NiftyLinkQThread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread *>(QThread::currentThread());
  assert(p);

  if (m_MySocket.IsNull())
  {
    QString errorMessage = QObject::tr("%1::DoProcessing() - Server socket is not initialised.").arg(objectName());
    QLOG_ERROR() << errorMessage;
    NiftyLinkStdExceptionMacro(std::logic_error, << errorMessage.toStdString());
  }

  NiftyLinkSocket::Pointer sock = NULL;

  // Loop continuously as we are in our own thread.
  while (this->GetIsRunning())
  {
    if (m_MySocket.IsNotNull() && (m_CommsSocket.IsNull() || !m_IsConnected))
    {
      NiftyLinkServerSocket::Pointer ssock = dynamic_cast<NiftyLinkServerSocket*>(m_MySocket.GetPointer());
      assert(ssock); // should always be true, as this class created it above.

      sock = ssock->WaitForConnection(m_ListenInterval);
      if (sock.IsNull() || !sock->GetConnected())
      {
        QLOG_INFO() << QObject::tr("%1::DoProcessing() - No client connecting").arg(objectName());
        continue;
      }

      // So by this point, client should be a valid new connection.
      assert(sock.IsNotNull());
      assert(sock->GetConnected());

      // Client connected, need to set socket parameters
      m_CommsSocket = sock;
      m_IsConnected = true;
      sock->SetTimeout(this->GetSocketTimeout());

      // Broadcast that we do have a valid connection.
      emit ClientConnectedSignal();

      // Startup timers.
      this->StartNoResponseTimer();
      this->StartKeepAliveTimer();
    }

    // Now we continuously process messages from the incoming receive buffer.
    this->ReceiveMessageLoop();

  } // end while - continuously checking for client connections.
}

} // end namespace niftk
