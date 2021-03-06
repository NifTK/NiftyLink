/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkTcpNetworkWorker.h"
#include <NiftyLinkMacro.h>
#include <NiftyLinkQThread.h>
#include <NiftyLinkUtils.h>
#include <NiftyLinkStringMessageHelpers.h>

#include <igtl_header.h>
#include <igtlMessageBase.h>
#include <igtlMessageFactory.h>
#include <igtlStatusMessage.h>

#include <QTcpSocket>
#include <QsLog.h>
#include <QTimer>

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpNetworkWorker::NiftyLinkTcpNetworkWorker(
    const QString& namePrefix,
    NiftyLinkMessageManager* inboundMessages,
    NiftyLinkMessageManager *outboundMessages,
    QTcpSocket *socket,
    QObject *parent)
: QObject(parent)
, m_Socket(socket)
, m_NamePrefix(namePrefix)
, m_MessagePrefix("")
, m_InboundMessages(inboundMessages)
, m_OutboundMessages(outboundMessages)
, m_HeaderInProgress(false)
, m_MessageInProgress(false)
, m_IncomingHeader(NULL)
, m_IncomingHeaderTimeStamp(NULL)
, m_TimeFullyReceivedTimeStamp(NULL)
, m_IncomingMessage(NULL)
, m_IncomingMessageBytesReceived(0)
, m_AbortReading(false)
, m_KeepAliveTimer(NULL)
, m_KeepAliveInterval(500)
, m_LastMessageSentTime(NULL)
, m_NoIncomingDataTimer(NULL)
, m_NoIncomingDataInterval(1000)
, m_LastMessageReceivedTime(NULL)
, m_Disconnecting(false)
{
  assert(m_Socket);
  assert(m_InboundMessages);
  assert(m_OutboundMessages);
  
  // These are expensive to create/destroy, so do it once.
  m_IncomingHeaderTimeStamp = igtl::TimeStamp::New();
  m_TimeFullyReceivedTimeStamp = igtl::TimeStamp::New();
  m_KeepAliveTimeStamp = igtl::TimeStamp::New();
  m_LastMessageSentTime = igtl::TimeStamp::New();
  m_NoIncomingDataTimeStamp = igtl::TimeStamp::New();
  m_LastMessageReceivedTime = igtl::TimeStamp::New();

  // Timers for internal monitoring.
  m_KeepAliveTimer = new QTimer(this);
  m_KeepAliveTimer->setInterval(m_KeepAliveInterval);

  m_NoIncomingDataTimer = new QTimer(this);
  m_NoIncomingDataTimer->setInterval(m_NoIncomingDataInterval);

  // Set object names for error messages.
  this->UpdateObjectName();

  connect(this, SIGNAL(InternalStatsSignal()), this, SLOT(OnOutputStats()));
  connect(this, SIGNAL(InternalSendSignal()), this, SLOT(OnSendMessage()), Qt::BlockingQueuedConnection);
  connect(this, SIGNAL(InternalDisconnectedSocketSignal()), this, SLOT(OnRequestSocketDisconnected()));
  connect(this, SIGNAL(InternalSetKeepAliveSignal(bool)), this, SLOT(OnSetKeepAliveOn(bool)));
  connect(this, SIGNAL(InternalSetCheckForNoIncomingDataSignal(bool)), this, SLOT(OnSetCheckForNoIncomingData(bool)));
  connect(m_NoIncomingDataTimer, SIGNAL(timeout()), this, SLOT(OnCheckForIncomingData()));
  connect(m_KeepAliveTimer, SIGNAL(timeout()), this, SLOT(OnSendInternalPing()));
  connect(m_Socket, SIGNAL(disconnected()), this, SLOT(OnSocketDisconnected()));
  connect(m_Socket, SIGNAL(bytesWritten(qint64)), this, SLOT(OnBytesSent(qint64)));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));
  connect(m_Socket, SIGNAL(readyRead()), this, SLOT(OnSocketReadyRead()));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpNetworkWorker() - created.").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
NiftyLinkTcpNetworkWorker::~NiftyLinkTcpNetworkWorker()
{
  QString name = m_MessagePrefix;
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpNetworkWorker() - destroying.").arg(name);

  // These objects created with this as parent. So Qt can clean them up when this object is deleted.
  m_KeepAliveTimer->stop();
  m_KeepAliveTimer->disconnect();
  m_NoIncomingDataTimer->stop();
  m_NoIncomingDataTimer->disconnect();

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpNetworkWorker() - destroyed.").arg(name);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::UpdateObjectName()
{
  assert(m_Socket);

  QString host = m_Socket->peerName();
  if (host.size() == 0)
  {
    host = "localhost";
  }

  m_MessagePrefix = QObject::tr("%1(d=%2, h=%3, p=%4)")
      .arg(m_NamePrefix).arg(m_Socket->socketDescriptor()).arg(host).arg(m_Socket->peerPort());

  this->setObjectName(m_MessagePrefix);
  m_ReceivedCounter.setObjectName(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
QTcpSocket* NiftyLinkTcpNetworkWorker::GetSocket() const
{
  return m_Socket;
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpNetworkWorker::IsSocketConnected() const
{
  return !m_Disconnecting && m_Socket != NULL && m_Socket->isOpen();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::RequestDisconnectSocket()
{
  QLOG_WARN() << QObject::tr("%1::RequestDisconnectSocket() - Requesting.").arg(objectName());

  // This is done, as this can be called from an external thread (eg. GUI thread),
  // but the processing of the request is done from the thread that this object is bound to (NiftyLinkQThread).
  emit InternalDisconnectedSocketSignal();

  QLOG_WARN() << QObject::tr("%1::RequestDisconnectSocket() - Requested.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  m_ReceivedCounter.SetNumberMessageReceivedThreshold(threshold);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::SetKeepAliveOn(bool isOn)
{
  emit InternalSetKeepAliveSignal(isOn);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSetKeepAliveOn(bool isOn)
{
  if (isOn)
  {
    m_KeepAliveTimer->start();
  }
  else
  {
    m_KeepAliveTimer->stop();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::SetCheckForNoIncomingData(bool isOn)
{
  emit InternalSetCheckForNoIncomingDataSignal(isOn);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSetCheckForNoIncomingData(bool isOn)
{
  if (isOn)
  {
    m_NoIncomingDataTimer->start();
  }
  else
  {
    m_NoIncomingDataTimer->stop();
  }
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpNetworkWorker::Send(NiftyLinkMessageContainer::Pointer message)
{
  if (!this->IsSocketConnected())
  {
    return false;
  }

  // This is done, as this can be called from an external thread (eg. GUI thread),
  // but the sending of the messages is done from the thread that this object is bound to (NiftyLinkQThread).
  m_OutboundMessages->InsertContainer(m_Socket->peerPort(), message);
  emit this->InternalSendSignal();

  return true;
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpNetworkWorker::RequestStats()
{
  // This is a convenience method so client code doesn't have to know
  // what this message format is. So, they can call RequestStats, and then
  // this method calls the Send method above.

  NiftyLinkMessageContainer::Pointer msg = niftk::CreateStringMessage(
      "NiftyLinkTcpNetworkWorker", m_Socket->peerName(), m_Socket->peerPort(), "STATS", m_LastMessageSentTime);

  return this->Send(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OutputStatsToConsole()
{
  // This is done, as this can be called from an external thread (eg. GUI thread),
  // but the processing of the request is done from the thread that this object is bound to (NiftyLinkQThread).
  emit this->InternalStatsSignal();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnRequestSocketDisconnected()
{
  if (this->IsSocketConnected())
  {
    QLOG_WARN() << QObject::tr("%1::OnRequestSocketDisconnected() - asking socket.").arg(objectName());
    m_Socket->disconnectFromHost();
    QLOG_WARN() << QObject::tr("%1::OnRequestSocketDisconnected() - asked socket.").arg(objectName());
  }
  else
  {
    QLOG_WARN() << QObject::tr("%1::OnRequestSocketDisconnected() - socket wasn't actually connected, so nothing to do.")
                   .arg(objectName());
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketDisconnected()
{
  QLOG_INFO() << QObject::tr("%1::OnSocketDisconnected() - starting to disconnect.").arg(objectName());

  emit SocketDisconnected();
  NiftyLinkQThread::SleepCallingThread(2000);

  m_Disconnecting = true;

  m_Socket->disconnect(); // i.e. disconnect Qt signals/slots, not TCP socket disconnect.
  m_Socket->deleteLater();

  this->disconnect();     // i.e. disconnect Qt signals/slots.
  this->deleteLater();

  this->ShutdownThread();

  QLOG_INFO() << QObject::tr("%1::OnSocketDisconnected() - disconnected.").arg(objectName());
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketError(QAbstractSocket::SocketError error)
{
  QString errorString = m_Socket->errorString();
  QLOG_ERROR() << QObject::tr("%1::OnSocketError(code=%2, string=%3).").arg(objectName()).arg(error).arg(errorString);
  emit SocketError(m_Socket->peerPort(), error, errorString);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnOutputStats()
{
  m_ReceivedCounter.OnOutputStats();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnCheckForIncomingData()
{
  m_NoIncomingDataTimeStamp->GetTime();
  double millisecondsSinceLastData = niftk::GetDifferenceInNanoSeconds(m_NoIncomingDataTimeStamp, m_LastMessageReceivedTime) / static_cast<double>(1000000);

  if (millisecondsSinceLastData > m_KeepAliveInterval)
  {
    QLOG_DEBUG() << QObject::tr("%1::OnCheckForIncomingData() - signalling 'NoIncomingData'.").arg(m_MessagePrefix);
    emit NoIncomingData();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnBytesSent(qint64 bytes)
{
  emit BytesSent(bytes);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketReadyRead()
{
  // This doubly double checks we are running in our own thread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  // This indicates a catastrophic failure, and hence no point continuing.
  if (m_AbortReading)
  {
    QLOG_ERROR() << QObject::tr("%1::OnSocketReadyRead() - Abort reading. Giving up.").arg(m_MessagePrefix);
    return;
  }

  // This indicates a catastrophic failure, and hence no point continuing.
  if (m_Disconnecting)
  {
    QLOG_WARN() << QObject::tr("%1::OnSocketReadyRead() - Shutting down, so stopping reading.").arg(m_MessagePrefix);
    return;
  }

  // Create stream outside of loop ... maybe more efficient.
  QDataStream in(m_Socket);
  in.setVersion(QDataStream::Qt_4_0);
  
  // Storing this locally, incase the socket gets updated.
  quint64 bytesAvailable = m_Socket->bytesAvailable();
  quint64 bytesReceived = 0;

  QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - Starting to read data, bytes available=%2.")
                  .arg(m_MessagePrefix).arg(bytesAvailable);

  // Need to cater for reading > 1 message at once.
  // Also need to cater for reading partial messages, as TCP may fragment them.
  do
  {
    if (!m_HeaderInProgress)
    {
      // If there are not enough bytes even for a header, we can wait until the rest of the message appears.
      if (bytesAvailable < IGTL_HEADER_SIZE)
      {
        QLOG_INFO() << QObject::tr("%1::OnSocketReadyRead() - Not enough bytes to create a header. Wait until next signal.")
                       .arg(m_MessagePrefix);
        return;
      }

      // Allocate a new header, which destroys the previous one, if no one else holds a smart-pointer to it.
      m_IncomingHeader = igtl::MessageHeader::New();
      m_IncomingHeader->InitPack();
      m_IncomingHeaderTimeStamp->GetTime();

      // Read header data.
      bytesReceived = in.readRawData(static_cast<char*>(m_IncomingHeader->GetPackPointer()), m_IncomingHeader->GetPackSize());
      if (bytesReceived <= 0)
      {
        m_AbortReading = true;

        QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to receive valid OpenIGTLink header. This suggests junk on the wire.")
            .arg(m_MessagePrefix);

        QLOG_ERROR() << errorMessage;
        emit SocketError(m_Socket->peerPort(), QAbstractSocket::UnknownSocketError, errorMessage);
        return;
      }

      // This should not occur, as we specifically checked for at least the right number of bytes for a header.
      assert(bytesReceived == m_IncomingHeader->GetPackSize());

      // Deserialize the header
      m_IncomingHeader->Unpack();
      m_HeaderInProgress = true;
      bytesAvailable -= bytesReceived;
    }

    // By this point we must have a header.
    assert(m_HeaderInProgress);
    assert(m_IncomingHeader.IsNotNull());

    // If the message already exists, we must be half way through receiving a message.
    if (!m_MessageInProgress)
    {
      try
      {
        // Allocate correct message type. The factory sets the header on the message and calls AllocatePack().
        igtl::MessageFactory::Pointer messageFactory = igtl::MessageFactory::New();
        m_IncomingMessage = messageFactory->GetMessage(m_IncomingHeader);
        m_IncomingMessageBytesReceived = 0;
        m_MessageInProgress = true;
      }
      catch (const std::exception& e)
      {
        m_AbortReading = true;

        QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to create message type %2. Error was %3. This suggests junk on the wire.")
            .arg(m_MessagePrefix).arg(QString::fromStdString(m_IncomingHeader->GetDeviceType())).arg(QString::fromStdString(e.what()));

        QLOG_ERROR() << errorMessage;
        emit SocketError(m_Socket->peerPort(), QAbstractSocket::UnknownSocketError, errorMessage);
        return;
      }
    }

    // By this point we must have a message.
    assert(m_MessageInProgress);
    assert(m_IncomingMessage.IsNotNull());

    // Check the size of the message.
    if (m_IncomingMessage->GetPackBodySize() > 0)
    {
      // Remember that we are incrementally building up the message by picking fragments off the socket.
      quint64 bytesRequiredToCompleteMessage = m_IncomingMessage->GetPackBodySize() - m_IncomingMessageBytesReceived;

      // If there are not enough bytes, we still want to read as much as possible to clear the read buffer.
      if (bytesAvailable < bytesRequiredToCompleteMessage)
      {
        QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - Message suggests there should be %2 bytes of data, but only %3 are available.")
                        .arg(m_MessagePrefix).arg(m_IncomingMessage->GetPackBodySize()).arg(bytesAvailable);

        bytesReceived = in.readRawData(static_cast<char *>(m_IncomingMessage->GetPackBodyPointer()) + m_IncomingMessageBytesReceived, bytesAvailable);
        if (bytesReceived <= 0 || bytesReceived != bytesAvailable)
        {
          m_AbortReading = true;

          QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to read the right size (%2) fragment, received (bytesReceived=%3).")
              .arg(m_MessagePrefix).arg(bytesAvailable).arg(bytesReceived);

          QLOG_ERROR() << errorMessage;
          emit SocketError(m_Socket->peerPort(), QAbstractSocket::UnknownSocketError, errorMessage);
          return;
        }
        m_IncomingMessageBytesReceived += bytesReceived;
        bytesAvailable -= bytesReceived;
        return;
      }
      else
      {
        // Receive remaining data from the socket.
        bytesReceived = in.readRawData(static_cast<char *>(m_IncomingMessage->GetPackBodyPointer())
                                       + m_IncomingMessageBytesReceived, bytesRequiredToCompleteMessage);

        if (bytesReceived <= 0 || bytesReceived != bytesRequiredToCompleteMessage)
        {
          m_AbortReading = true;

          QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to read the right size (%2) OpenIGTLink message, (bytesReceived=%3).")
              .arg(m_MessagePrefix)
              .arg(bytesRequiredToCompleteMessage)
              .arg(bytesReceived);

          QLOG_ERROR() << errorMessage;
          emit SocketError(m_Socket->peerPort(), QAbstractSocket::UnknownSocketError, errorMessage);
          return;
        }
        m_IncomingMessageBytesReceived += bytesReceived;
        bytesAvailable -= bytesReceived;
      }

      // Don't forget to Unpack!
      m_IncomingMessage->Unpack();

      bool isKeepAlive = niftk::IsKeepAlive(m_IncomingMessage);
      if (isKeepAlive)
      {
        QLOG_DEBUG() << QObject::tr("%1::IsKeepAlive() - received STATUS_OK as keep-alive.").arg(m_MessagePrefix);
      }

      bool isStatsRequest = niftk::IsStatsRequest(m_IncomingMessage);
      if (isStatsRequest)
      {
        QLOG_DEBUG() << QObject::tr("%1::IsStatsRequest() - received request for statistics.").arg(m_MessagePrefix);
        this->OnOutputStats();
      }

      // Check for special case messages. They are squashed here, and not delivered to client.
      if (isKeepAlive || isStatsRequest)
      {
        
        m_LastMessageReceivedTime->GetTime();
        
        // Throw away the message and header, and bail out.
        m_HeaderInProgress = false;
        m_MessageInProgress = false;
        m_IncomingHeader = NULL;
        m_IncomingMessage = NULL;
        m_IncomingMessageBytesReceived = 0;
        continue;
      }
    } // end if we have a body.

    m_TimeFullyReceivedTimeStamp->GetTime();

    // This is the container we eventually publish.
     NiftyLinkMessageContainer::Pointer msg = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));

    // Set timestamps on NiftyLink container.
    msg->SetTimeArrived(m_IncomingHeaderTimeStamp);
    msg->SetTimeReceived(m_TimeFullyReceivedTimeStamp);
    msg->SetMessage(m_IncomingMessage);

    QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - id=%2, class=%3, size=%4 bytes, device='%5', avail=%6")
                    .arg(m_MessagePrefix)
                    .arg(msg->GetNiftyLinkMessageId())
                    .arg(m_IncomingMessage->GetNameOfClass())
                    .arg(m_IncomingMessage->GetPackSize())
                    .arg(m_IncomingMessage->GetDeviceType())
                    .arg(bytesAvailable)
                    ;

    // We now reset these variables.
    // We don't reset the smart pointers, as we publish message to rest of application.
    m_HeaderInProgress = false;
    m_MessageInProgress = false;
    m_IncomingMessageBytesReceived = 0;

    // For stats.
    m_ReceivedCounter.OnMessageReceived(msg);

    // For monitoring.
    m_LastMessageReceivedTime->GetTime();

    // Store the message in the map, and signal that we have done so.
    m_InboundMessages->InsertContainer(m_Socket->peerPort(), msg);
    emit MessageReceived(m_Socket->peerPort());

  } while (bytesAvailable > 0);

  return;
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSendMessage()
{
  // This doubly double checks we are running in our own thread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  NiftyLinkMessageContainer::Pointer message = m_OutboundMessages->GetContainer(m_Socket->peerPort());
  igtl::MessageBase::Pointer msg = message->GetMessage();
  this->InternalSendMessage(msg);

  QLOG_DEBUG() << QObject::tr("%1::OnSendMessage() - sent.").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSendInternalPing()
{
  // This doubly double checks we are running in our own thread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  // Check if we can avoid sending this message, just to save on network traffic.
  m_KeepAliveTimeStamp->GetTime();
  igtlUint64 diff = GetDifferenceInNanoSeconds(m_KeepAliveTimeStamp, m_LastMessageSentTime) / 1000000; // convert nano to milliseconds.
  if (diff < m_KeepAliveInterval/2)
  {
    QLOG_DEBUG() << QObject::tr("%1::OnSendInternalPing() - No real need to send a keep-alive as we have recently processed a message.").arg(objectName());
    return;
  }

  // Note: This is deliberately the same as in PLUS.
  igtl::StatusMessage::Pointer msg = igtl::StatusMessage::New();
  msg->SetCode(igtl::StatusMessage::STATUS_OK);
  msg->Pack();

  this->InternalSendMessage(msg.GetPointer());
  emit SentKeepAlive();

  QLOG_DEBUG() << QObject::tr("%1::OnSendInternalPing() - sent.").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::InternalSendMessage(igtl::MessageBase::Pointer msg)
{
  // This doubly double checks we are running in our own thread.
  niftk::NiftyLinkQThread *p = dynamic_cast<niftk::NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  if (!this->IsSocketConnected())
  {
    QLOG_ERROR() << QObject::tr("%1::SendMessage() - Socket appears to be unconnected.").arg(m_MessagePrefix);
    return;
  }

  int bytesWritten = m_Socket->write(static_cast<const char*>(msg->GetPackPointer()), msg->GetPackSize());
  if (bytesWritten != msg->GetPackSize())
  {
    QLOG_ERROR() << QObject::tr("%1::SendMessage() - only written %2 bytes instead of %3").arg(m_MessagePrefix).arg(bytesWritten).arg(msg->GetPackSize());
  }

  // Store the time where we last sent a message.
  m_LastMessageSentTime->GetTime();

  QLOG_DEBUG() << QObject::tr("%1::SendMessage() - written %2 bytes.").arg(m_MessagePrefix).arg(bytesWritten);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::ShutdownThread()
{
  // This doubly double checks we are running in our own NiftyLinkQThread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread*>(this->thread());
  assert(p != NULL);

  // We only need to try shutting down, if we have not already finished.
  if (!p->isFinished())
  {
    p->exit(0);
  }
}

} // end niftk namespace
