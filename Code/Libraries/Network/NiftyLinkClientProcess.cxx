/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkClientProcess.h"
#include <NiftyLinkQThread.h>
#include <NiftyLinkClientSocket.h>

#include <QsLog.h>

#include <stdexcept>
#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkClientProcess::NiftyLinkClientProcess()
: m_ConnectionTimeout(5000)
, m_Port(0)
, m_HostName("")
{
  this->setObjectName("NiftyLinkClientProcess");
}


//-----------------------------------------------------------------------------
NiftyLinkClientProcess::~NiftyLinkClientProcess()
{
}


//-----------------------------------------------------------------------------
void NiftyLinkClientProcess::SetConnectionTimeOut(const int& msec)
{
  m_ConnectionTimeout = msec;
}


//-----------------------------------------------------------------------------
int NiftyLinkClientProcess::GetConnectionTimeOut()
{
  return m_ConnectionTimeout;
}


//-----------------------------------------------------------------------------
void NiftyLinkClientProcess::Initialise(const QString &hostname, const int &port)
{
  if (port <= 0 || hostname.size() == 0)
  {
    QString errorMessage = QObject::tr("%1::Initialise(%2, %3) - Cannot create a client socket, invalid host or port specified.").arg(objectName()).arg(hostname).arg(port);

    QLOG_ERROR() << errorMessage;
    throw std::invalid_argument(errorMessage.toStdString());
  }

  if (m_MySocket.IsNotNull() || m_CommsSocket.IsNotNull())
  {
    QString errorMessage = QObject::tr("%1::Initialise(%2, %3) - Client already in use, you should not try to re-initialise it.").arg(objectName()).arg(hostname).arg(port);

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  this->m_MySocket = NiftyLinkClientSocket::New();  // Note: these are deliberately the same.
  this->m_CommsSocket = m_MySocket;                 // Note: these are deliberately the same.
  this->m_HostName = hostname;
  this->m_Port = port;

  QLOG_INFO() << QObject::tr("%1::Initialise(%2, %3) - Successfully created (but not yet opened).").arg(objectName()).arg(hostname).arg(port);
}


//-----------------------------------------------------------------------------
void NiftyLinkClientProcess::DoProcessing()
{
  // We MUST be running within NiftyLinkQThread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread *>(QThread::currentThread());
  assert(p);

  if (m_MySocket.IsNull())
  {
    QString errorMessage = QObject::tr("%1::DoProcessing() - Client socket is not initialised.").arg(objectName());

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  if (m_CommsSocket.IsNull())
  {
    QString errorMessage = QObject::tr("%1::DoProcessing() - Client socket is not initialised.").arg(objectName());

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  NiftyLinkClientSocket::Pointer csock = dynamic_cast<NiftyLinkClientSocket*>(m_MySocket.GetPointer());
  assert(csock); // should always be true, as this class created it above.

  int err = csock->ConnectToServer(m_HostName.toStdString().c_str(), m_Port);
  if (err < 0)
  {
    QString errorMessage = QObject::tr("%1::DoProcessing() - Error connecting to server (%2:%3). Check console/log file.").arg(objectName()).arg(m_HostName).arg(m_Port);
    QLOG_ERROR() << errorMessage;

    throw std::runtime_error(errorMessage.toStdString());
  }

  m_MySocket->SetTimeout(this->GetSocketTimeout());
  csock->SetConnectionTimeout(this->GetConnectionTimeOut());
  m_IsConnected = true;

  // For logging.
  this->setObjectName(QObject::tr("NiftyLinkClientProcess(%1:%2)").arg(m_HostName).arg(m_Port));

  // Broadcast that we do have a valid connection.
  emit ConnectedToRemote();

  QLOG_INFO() << QObject::tr("%1::Initialise(%2, %3) - Should be connected by now.").arg(objectName()).arg(m_HostName).arg(m_Port);

  // Startup timers.
  this->StartNoResponseTimer();
  this->StartKeepAliveTimer();

  // Now we continuously process messages from the incoming receive buffer.
  this->ReceiveMessageLoop();
}

} // end namespace niftk
