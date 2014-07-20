/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkNetworkProcess.h"
#include <NiftyLinkSocket.h>
#include <NiftyLinkQThread.h>
#include <NiftyLinkUtils.h>

#include <QTimer>
#include <QsLog.h>
#include <QCoreApplication>

#include <igtlStringMessage.h>

#include <stdexcept>
#include <iostream>
#include <cassert>

namespace niftk
{

const std::string NiftyLinkNetworkProcess::KEEP_ALIVE_MESSAGE("POKE");

//-----------------------------------------------------------------------------
NiftyLinkNetworkProcess::NiftyLinkNetworkProcess(QObject *parent)
: m_NumberOfMessagesReceived(0)
, m_NumberOfMessagesSent(0)
, m_SocketTimeout(50)
, m_SleepInterval(0)
, m_MySocket(NULL)
, m_CommsSocket(NULL)
, m_KeepAliveTimer(NULL)
, m_KeepAliveInterval(500)
, m_NoResponseTimer(NULL)
, m_NoResponseInterval(2000)
, m_IsRunning(false)
, m_IsConnected(false)
, m_LastMessageProcessedTime(NULL)
{
  m_LastMessageProcessedTime = igtl::TimeStamp::New();
}


//-----------------------------------------------------------------------------
NiftyLinkNetworkProcess::~NiftyLinkNetworkProcess()
{
  this->TerminateProcess();
}


//-----------------------------------------------------------------------------
bool NiftyLinkNetworkProcess::GetIsRunning() const
{
  return m_IsRunning;
}


//-----------------------------------------------------------------------------
int NiftyLinkNetworkProcess::GetSocketTimeout() const
{
  return m_SocketTimeout;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::SetSocketTimeout(const int& msec)
{
  m_SocketTimeout = msec;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::SetSleepInterval(const int& msec)
{
  m_SleepInterval = msec;
}


//-----------------------------------------------------------------------------
int NiftyLinkNetworkProcess::GetSleepInterval() const
{
  return m_SleepInterval;
}


//-----------------------------------------------------------------------------
int NiftyLinkNetworkProcess::GetKeepAliveInterval() const
{
  return m_KeepAliveInterval;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::SetKeepAliveInterval(const int& msec)
{
  m_KeepAliveInterval = msec;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::StartKeepAliveTimer()
{
  if (m_KeepAliveTimer == NULL)
  {
    QString errorMessage = QObject::tr("%1::NiftyLinkNetworkProcess::StartKeepAliveTimer() - QTimer is not initialised, so can't start it.").arg(objectName());

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  m_KeepAliveTimer->start(m_KeepAliveInterval);
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::StopKeepAliveTimer()
{
  if (m_KeepAliveTimer == NULL)
  {
    QString errorMessage = QObject::tr("%1::NiftyLinkNetworkProcess::StopKeepAliveTimer() - QTimer is not initialised, so can't stop it.").arg(objectName());

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  m_KeepAliveTimer->stop();
}


//-----------------------------------------------------------------------------
int NiftyLinkNetworkProcess::GetNoResponseInterval() const
{
  return m_NoResponseInterval;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::SetNoResponseInterval(const int& msec)
{
  m_NoResponseInterval = msec;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::StartNoResponseTimer()
{
  if (m_NoResponseTimer == NULL)
  {
    QString errorMessage = QObject::tr("%1::StartNoResponseTimer() - QTimer is not initialised, so can't start it.").arg(objectName());

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  m_NoResponseTimer->start(m_NoResponseInterval);
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::StopNoResponseTimer()
{
  if (m_NoResponseTimer == NULL)
  {
    QString errorMessage = QObject::tr("%1::StopNoResponseTimer() - QTimer is not initialised, so can't stop it.").arg(objectName());

    QLOG_ERROR() << errorMessage;
    throw std::logic_error(errorMessage.toStdString());
  }

  m_NoResponseTimer->stop();
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::StartProcessing()
{
  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::StartProcessing() - starting").arg(objectName());

  // See QThread manual page. Using signal to launch
  // within the context of another thread, hence another event loop.
  connect(this, SIGNAL(InternalStartWorkingSignal()), this, SLOT(RunProcessing()));

  m_IsRunning = true;
  emit InternalStartWorkingSignal();

  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::StartProcessing() - finishing").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::StopProcessing()
{
  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::StopProcessing() - requested").arg(objectName());

  // Here we set the member variable to false, and the processing loops in subclass will realise to exit.
  m_IsRunning = false;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::RunProcessing()
{
  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::RunProcessing() - starting.").arg(objectName());

  // We MUST be running within NiftyLinkQThread...
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread *>(QThread::currentThread());
  assert(p);

  // .. so that these timers then bind to the new thread, and run within the threads event loop.
  m_KeepAliveTimer = new QTimer();
  m_KeepAliveTimer->setInterval(m_KeepAliveInterval);
  connect(m_KeepAliveTimer, SIGNAL(timeout()), this, SLOT(OnKeepAliveTimerTimedOut()));

  m_NoResponseTimer = new QTimer();
  m_NoResponseTimer->setInterval(m_NoResponseInterval);
  connect(m_NoResponseTimer, SIGNAL(timeout()), this, SLOT(OnNoResponseTimerTimedOut()));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::RunProcessing() - calling DoProcessing() in sub-class.").arg(objectName());
  this->DoProcessing();

  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::RunProcessing() - finished DoProcessing() in sub-class, tidying up.").arg(objectName());
  this->TerminateProcess();

  QLOG_INFO() << QObject::tr("%1::NiftyLinkNetworkProcess::RunProcessing() - finished.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::ReceiveMessageLoop()
{
  assert(m_CommsSocket.IsNotNull());

  // We MUST be running within NiftyLinkQThread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread *>(QThread::currentThread());
  assert(p);

  // We loop, continuously checking for arriving data.
  while (this->GetIsRunning() && m_IsConnected)
  {
    // Process data from socket.
    int bytesPending = m_CommsSocket->CheckPendingData();
    if (bytesPending < 0)
    {
      QString errorMessage = QObject::tr("%1::ReceiveMessageLoop() - CheckPendingData returned -1. This is a system error. Check console/log file.").arg(objectName());

      QLOG_ERROR() << errorMessage;
      throw std::runtime_error(errorMessage.toStdString());
    }

    // Process the message.
    if (bytesPending > 0)
    {
      this->ReceiveMessage();
    }

    // Slow loop down a bit. Not advisable.
    if (m_SleepInterval > 0)
    {
      p->MsleepEx(m_SleepInterval);
    }

    // Make sure this while loop doesn't swamp the QTimers.
    // Make sure this is at end of loop, so in preference we send messages first.
    QCoreApplication::processEvents();

  } // end while - continuously processing data.
}


//-----------------------------------------------------------------------------
bool NiftyLinkNetworkProcess::ReceiveMessage()
{
  assert(m_CommsSocket.IsNotNull());

  // This is the container we eventually publish.
  NiftyLinkMessageContainer::Pointer msg;

  // This contains the original OpenIGTLink message.
  igtl::MessageBase::Pointer message;

  // Create a message buffer to receive header
  igtl::MessageHeader::Pointer msgHeader = igtl::MessageHeader::New();
  msgHeader->InitPack();

  // Receive generic header from the socket
  int r = m_CommsSocket->Receive(msgHeader->GetPackPointer(), msgHeader->GetPackSize());
  if (r <= 0)
  {
    // Failed to read a proper header. Is this an error? If its not an OpenIGTLink message we discard it anyway.
    QLOG_WARN() << QObject::tr("%1::ReceiveMessageLoop() - Failed to receive valid OpenIGTLink header.").arg(objectName());
    return false;
  }
  if (r != msgHeader->GetPackSize())
  {
    // Failed to read a proper header. Is this an error? If its not an OpenIGTLink message we discard it anyway.
    QLOG_WARN() << QObject::tr("%1::ReceiveMessageLoop() - Failed to read the right size OpenIGTLink header.").arg(objectName());
    return false;
  }

  // Restart the timer, as we may be receiving a header only
  // message, so we wont do it in the body section below.
  this->StartNoResponseTimer();

  // Deserialize the header
  msgHeader->Unpack();

  // Allocate correct message type.
  std::string type = msgHeader->GetDeviceType();
  if (type == "STRING")
  {
    message = igtl::StringMessage::New();
  }
  message->SetMessageHeader(msgHeader);
  message->AllocatePack();

  // Create a new timestamp.
  igtl::TimeStamp::Pointer timeReceived  = igtl::TimeStamp::New();

  // Check the size of the message.
  if (message->GetPackBodySize() > 0)
  {
    // Receive data from the socket
    r = m_CommsSocket->Receive(message->GetPackBodyPointer(), message->GetPackBodySize());
    if (r <= 0)
    {
      // Failed to read message.
      QLOG_WARN() << QObject::tr("%1::ReceiveMessageLoop() - Failed to receive valid OpenIGTLink message (r=%2).").arg(objectName()).arg(r);
      return false;
    }

    // Don't forget to Unpack!
    message->Unpack();

    // Message fully received
    timeReceived->GetTime();

    // So we restart the no response timer, as we have had a valid message.
    this->StartNoResponseTimer();
  }

  // Check if its a string keep-alive message.
  // These are not delivered to the client via the Qt signal. They are squashed here.
  igtl::StringMessage::Pointer tmp = dynamic_cast<igtl::StringMessage*>(message.GetPointer());
  if (tmp.IsNotNull() && tmp->GetString() == KEEP_ALIVE_MESSAGE)
  {
    QLOG_DEBUG() << QObject::tr("%1::ReceiveMessageLoop() - Keep-alive received, restarting the timeouter.").arg(objectName());
    this->StartNoResponseTimer();
    return true;
  }

  // Get the receive timestamp from the socket - marks when the first byte of the package arrived
  igtl::TimeStamp::Pointer timeArrived = igtl::TimeStamp::New();

  // TODO: Two socket implementations are not quite equivalent.
  if (dynamic_cast<NiftyLinkSocket*>(m_CommsSocket.GetPointer()))
  {
    timeArrived->SetTimeInNanoseconds(dynamic_cast<NiftyLinkSocket*>(m_CommsSocket.GetPointer())->GetReceiveTimestampInNanoseconds());
  }
  else
  {
    timeArrived->SetTimeInNanoseconds(timeReceived->GetTimeStampInNanoseconds());
  }

  // Set timestamps on NiftyLink container.
  msg->SetTimeArrived(timeArrived);
  msg->SetTimeReceived(timeReceived);
  msg->SetMessage(message);

  m_NumberOfMessagesReceived++;
  m_LastMessageProcessedTime->SetTimeInNanoseconds(timeArrived->GetTimeStampInNanoseconds());

  QLOG_INFO() << objectName() <<"::" << "ReceiveMessage(): " \
              << msg->GetNiftyLinkMessageId() \
              << ", " << message->GetNameOfClass() \
              << ", " << message->GetPackBodySize() << " bytes.";

  emit MessageReceived(msg);

  return true;
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::Send(igtl::MessageBase::Pointer msg)
{
  assert(m_CommsSocket.IsNotNull());

  if (this->GetIsRunning())
  {
    igtl::TimeStamp::Pointer sendStarted = igtl::TimeStamp::New();
    msg->SetTimeStamp(sendStarted);

    int ret = m_CommsSocket->Send(msg->GetPackPointer(), msg->GetPackSize());
    if (ret <= 0)
    {
      QString errorMessage = QObject::tr("%1::Send() - Failed to send message, return code %2. Check console/log file.").arg(objectName()).arg(ret);
      QLOG_ERROR() << errorMessage;

      throw std::logic_error(errorMessage.toStdString());
    }

    // Emit the time. Note, this assumes the constructor sets a valid time.
    igtl::TimeStamp::Pointer sendFinished = igtl::TimeStamp::New();

    // Internal message counter
    m_NumberOfMessagesSent++;

    // Store the time for later, as its used to try and avoid sending too many keep-alive messages.
    m_LastMessageProcessedTime->SetTimeInNanoseconds(sendFinished->GetTimeStampInNanoseconds());

    emit MessageSent(sendStarted->GetTimeStampInNanoseconds(),
                     sendFinished->GetTimeStampInNanoseconds());
  }
}



//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::OnKeepAliveTimerTimedOut()
{
  QLOG_DEBUG() << QObject::tr("%1::OnKeepAliveTimerTimedOut() - started").arg(objectName());

  assert(m_CommsSocket.IsNotNull());

  if (!this->GetIsRunning())
  {
    // Note necessarily an error? Depends on timing?
    QLOG_INFO() << QObject::tr("%1::OnKeepAliveTimerTimedOut() - Called when process is not yet fully running").arg(objectName());
    return;
  }

  if (!this->m_IsConnected)
  {
    // Note necessarily an error? Depends on timing?
    QLOG_INFO() << QObject::tr("%1::OnKeepAliveTimerTimedOut() - Called when process is not yet fully connected").arg(objectName());
    return;
  }

  igtl::TimeStamp::Pointer sendStarted = igtl::TimeStamp::New();

  // Check if we can avoid sending this message, just to save on network traffic.
  igtlUint64 diff = GetDifferenceInNanoSeconds(sendStarted, m_LastMessageProcessedTime) / 1000000; // convert nano to milliseconds.
  if (diff < m_KeepAliveInterval)
  {
    QLOG_INFO() << QObject::tr("%1::OnKeepAliveTimerTimedOut() - No real need to send a keep-alive as we have recently processed a message.").arg(objectName());
    return;
  }

  std::string tmpMsg(KEEP_ALIVE_MESSAGE);
  igtl::StringMessage::Pointer stringMessage = igtl::StringMessage::New();
  stringMessage->SetDeviceName(objectName().toStdString().c_str());
  stringMessage->SetString(tmpMsg);
  stringMessage->SetTimeStamp(sendStarted);
  stringMessage->Pack();

  int rval = m_CommsSocket->Send(stringMessage->GetPackPointer(), stringMessage->GetPackSize());
  if (rval != 1)
  {
    QLOG_ERROR() << QObject::tr("%1::OnKeepAliveTimerTimedOut() - Cannot send keep-alive message: I am probably disconnected from remote host.").arg(objectName());
    this->StopNoResponseTimer();
    this->StopKeepAliveTimer();
    m_IsConnected = false;

    emit CantReachRemote();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::OnNoResponseTimerTimedOut()
{
  if (!this->GetIsRunning())
  {
    // Note necessarily an error? Depends on timing?
    QLOG_INFO() << QObject::tr("%1::OnNoResponseTimerTimedOut() - Called when process is not yet fully running").arg(objectName());
    return;
  }

  if (!this->m_IsConnected)
  {
    // Note necessarily an error? Depends on timing?
    QLOG_INFO() << QObject::tr("%1::OnNoResponseTimerTimedOut() - Called when process is not yet fully connected").arg(objectName());
    return;
  }

  QLOG_INFO() << QObject::tr("%1::OnNoResponseTimerTimedOut() - Other end disconnected ... stopping timers.").arg(objectName());
  this->StopNoResponseTimer();
  this->StopKeepAliveTimer();
  m_IsConnected = false;

  emit NoResponseFromRemote();
}


//-----------------------------------------------------------------------------
void NiftyLinkNetworkProcess::TerminateProcess()
{
  emit TerminateStarted();

  // We MUST be running within NiftyLinkQThread...
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread *>(QThread::currentThread());
  assert(p);

  m_IsRunning = false;
  int sleepInterval = 100; // milliseconds

  this->StopKeepAliveTimer();
  this->StopNoResponseTimer();

  delete m_KeepAliveTimer;
  m_KeepAliveTimer = NULL;

  delete m_NoResponseTimer;
  m_NoResponseTimer = NULL;

  // Quitting Process
  QLOG_INFO() << QObject::tr("%1::TerminateProcess() - Total number of messages received=%2, sent=%3").arg(objectName()).arg(m_NumberOfMessagesReceived).arg(m_NumberOfMessagesSent);

  if (m_CommsSocket.IsNotNull() && m_MySocket.IsNotNull() && m_CommsSocket != m_MySocket)
  {
    int err = m_CommsSocket->CloseSocket();
    if (err == -1)
    {
      QLOG_ERROR() << QObject::tr("%1::TerminateProcess() - failed to close client side of server socket, call returned %2, check log file.").arg(objectName()).arg(err);
    }
    QLOG_INFO() << QObject::tr("%1::TerminateProcess() - Closing client side returned %2, now sleeping for %3 msec.").arg(objectName()).arg(err).arg(sleepInterval);
    p->MsleepEx(sleepInterval);
  }

  if (m_MySocket.IsNotNull())
  {
    int err = m_MySocket->CloseSocket();
    if (err == -1)
    {
      QLOG_ERROR() << QObject::tr("%1::TerminateProcess() - failed to close socket, call returned %2, check log file.").arg(objectName()).arg(err);
    }
    QLOG_INFO() << QObject::tr("%1::TerminateProcess() - Closing socket returned %2, now sleeping for %3 msec.").arg(objectName()).arg(err).arg(sleepInterval);
    p->MsleepEx(sleepInterval);
  }
  m_IsConnected = false;

  emit TerminateFinished();
}

} // end namespace niftk
