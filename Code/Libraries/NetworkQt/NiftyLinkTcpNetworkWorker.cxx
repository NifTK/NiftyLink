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

#include <igtl_header.h>
#include <igtlMessageBase.h>
#include <igtlMessageFactory.h>
#include <igtlStringMessage.h>

#include <QTcpSocket>
#include <QsLog.h>
#include <QTimer>

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkTcpNetworkWorker::NiftyLinkTcpNetworkWorker(
    NiftyLinkMessageManager* inboundMessages,
    NiftyLinkMessageManager *outboundMessages,
    QTcpSocket *socket,
    QObject *parent)
: QObject(parent)
, m_InboundMessages(inboundMessages)
, m_OutboundMessages(outboundMessages)
, m_Socket(socket)
, m_HeaderInProgress(false)
, m_MessageInProgress(false)
, m_IncomingHeader(NULL)
, m_IncomingHeaderTimeStamp(NULL)
, m_IncomingMessage(NULL)
, m_IncomingMessageBytesReceived(0)
, m_AbortReading(false)
, m_KeepAliveTimer(NULL)
, m_KeepAliveInterval(1000)
, m_LastMessageSentTime(NULL)
, m_NoIncomingDataTimer(NULL)
, m_NoIncomingDataInterval(2000)
, m_LastMessageReceivedTime(NULL)
{
  assert(m_Socket);
  assert(m_InboundMessages);
  assert(m_OutboundMessages);

  QString host = m_Socket->peerName();
  if (host.size() == 0)
  {
    host = "localhost";
  }

  m_KeepAliveTimer = new QTimer();
  m_KeepAliveTimer->setInterval(m_KeepAliveInterval);
  m_LastMessageSentTime = igtl::TimeStamp::New();

  m_NoIncomingDataTimer = new QTimer();
  m_NoIncomingDataTimer->setInterval(m_NoIncomingDataInterval);
  m_LastMessageReceivedTime = igtl::TimeStamp::New();

  m_MessagePrefix = QObject::tr("NiftyLinkTcpNetworkWorker(d=%1, h=%2, p=%3)").arg(m_Socket->socketDescriptor()).arg(host).arg(m_Socket->peerPort());
  this->setObjectName(m_MessagePrefix);
  m_ReceivedCounter.setObjectName(m_MessagePrefix);

  connect(this, SIGNAL(InternalStatsSignal()), this, SLOT(OnOutputStats()), Qt::BlockingQueuedConnection);
  connect(this, SIGNAL(InternalSendSignal()), this, SLOT(OnSend()), Qt::BlockingQueuedConnection);
  connect(m_NoIncomingDataTimer, SIGNAL(timeout()), this, SLOT(OnCheckForIncomingData()));
  connect(m_KeepAliveTimer, SIGNAL(timeout()), this, SLOT(OnSendInternalPing()));
  connect(m_Socket, SIGNAL(bytesWritten(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Socket, SIGNAL(disconnected()), this, SLOT(OnSocketDisconnected()));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));
  connect(m_Socket, SIGNAL(readyRead()), this, SLOT(OnSocketReadyRead()));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpNetworkWorker() - created.").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
NiftyLinkTcpNetworkWorker::~NiftyLinkTcpNetworkWorker()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpNetworkWorker() - destroying.").arg(m_MessagePrefix);

  if (m_KeepAliveTimer != NULL)
  {
    m_KeepAliveTimer->disconnect();
    delete m_KeepAliveTimer;
  }

  if (m_NoIncomingDataTimer != NULL)
  {
    m_NoIncomingDataTimer->disconnect();
    delete m_NoIncomingDataTimer;
  }

  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpNetworkWorker() - destroyed.").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
QTcpSocket* NiftyLinkTcpNetworkWorker::GetSocket() const
{
  return m_Socket;
}


//-----------------------------------------------------------------------------
bool NiftyLinkTcpNetworkWorker::CloseSocket(QTcpSocket* socket)
{
  if(socket == NULL)
  {
    QLOG_ERROR() << QObject::tr("NiftyLinkTcpNetworkWorker::CloseSocket() - requesting to close a NULL socket?.");
    return false;
  }

  if (!socket->isOpen())
  {
    QLOG_DEBUG() << QObject::tr("NiftyLinkTcpNetworkWorker::CloseSocket() - requesting to close a closed socket?.");
    return false;
  }

  socket->close();
  while(socket->isOpen())
  {
    QThread::currentThread()->wait(10);
  }

  return !socket->isOpen();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::SetNumberMessageReceivedThreshold(qint64 threshold)
{
  m_ReceivedCounter.SetNumberMessageReceivedThreshold(threshold);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::SetKeepAliveOn(bool isOn)
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
  if (!m_Socket->isOpen() || !m_Socket->isWritable())
  {
    return false;
  }

  m_OutboundMessages->InsertContainer(m_Socket->peerPort(), message);

  // This is done, as this can be called from an external thread (eg. GUI thread),
  // but the sending of the messages is done from the thread that this object is bound to (NiftyLinkQThread).
  emit this->InternalSendSignal();

  return true;
}



//-----------------------------------------------------------------------------
bool NiftyLinkTcpNetworkWorker::RequestStats()
{
  igtl::StringMessage::Pointer msg = igtl::StringMessage::New();
  msg->SetDeviceName("NiftyLinkTcpNetworkWorker");
  msg->SetString("STATS");
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName("NiftyLinkTcpNetworkWorker");
  m->SetSenderHostName(m_Socket->peerName());
  m->SetSenderPortNumber(m_Socket->peerPort());

  // Note: Calling Send(NiftyLinkMessageContainer::Pointer message) above.
  // So, the sending of the messages is done from the thread that this object is bound to (NiftyLinkQThread).
  return this->Send(m);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OutputStatsToConsole()
{
  // This is done, as this can be called from an external thread (eg. GUI thread),
  // but the processing of the request is done from the thread that this object is bound to (NiftyLinkQThread).
  emit this->InternalStatsSignal();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketDisconnected()
{
  QLOG_INFO() << QObject::tr("%1::OnSocketDisconnected() - starting.").arg(m_MessagePrefix);

  // If there are no socket events, then we might as well call it a day, and finish this thread.
  QCoreApplication::processEvents();
  QThread::currentThread()->quit();

  QLOG_INFO() << QObject::tr("%1::OnSocketDisconnected() - finished.").arg(m_MessagePrefix);
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
  igtl::TimeStamp::Pointer timeNow = igtl::TimeStamp::New();
  double millisecondsSinceLastData = niftk::GetDifferenceInNanoSeconds(timeNow, m_LastMessageReceivedTime) / static_cast<double>(1000000);
  if (millisecondsSinceLastData > m_KeepAliveInterval)
  {
    QLOG_DEBUG() << QObject::tr("%1::OnCheckForIncomingData() - signalling 'NoIncomingData'.").arg(m_MessagePrefix);
    emit NoIncomingData();
  }
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketReadyRead()
{
  // This indicates a catastrophic failure, and hence no point continuing.
  if (m_AbortReading)
  {
    QLOG_ERROR() << QObject::tr("%1::OnSocketReadyRead() - Abort reading. Giving up.").arg(m_MessagePrefix);
    return;
  }

  // This doubly double checks we are running in our own thread.
  NiftyLinkQThread *p = dynamic_cast<NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  // Create stream outside of loop ... maybe more efficient.
  QDataStream in(m_Socket);
  in.setVersion(QDataStream::Qt_4_0);

  // Storing this locally, incase the socket gets updated.
  quint64 bytesAvailable = m_Socket->bytesAvailable();
  quint64 bytesReceived = 0;

  QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - Starting to read data, bytes available=%2.").arg(m_MessagePrefix).arg(bytesAvailable);

  // Need to cater for reading > 1 message at once.
  // Also need to cater for reading partial messages, as TCP may fragment them.
  do
  {
    if (!m_HeaderInProgress)
    {
      // If there are not enough bytes even for a header, we can wait until the rest of the message appears.
      if (bytesAvailable < IGTL_HEADER_SIZE)
      {
        QLOG_INFO() << QObject::tr("%1::OnSocketReadyRead() - Not enough bytes to create a header. Wait until next signal.").arg(m_MessagePrefix);
        return;
      }

      // Allocate a new header, which destroys the previous one, if no one else holds a smart-pointer to it.
      m_IncomingHeader = igtl::MessageHeader::New();
      m_IncomingHeader->InitPack();

      // This assumes the constructor creates a valid timestamp.
      m_IncomingHeaderTimeStamp = igtl::TimeStamp::New();

      // Read header data.
      bytesReceived = in.readRawData(static_cast<char*>(m_IncomingHeader->GetPackPointer()), m_IncomingHeader->GetPackSize());
      if (bytesReceived <= 0)
      {
        m_AbortReading = true;
        QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to receive valid OpenIGTLink header. I expected to read a full header, but couldn't. This suggests junk on the wire, or system failure.").arg(m_MessagePrefix);
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
        QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to create message type %2. Error was %3. This suggests junk on the wire, or incorrectly encoded messages, or similar.").arg(m_MessagePrefix).arg(QString::fromStdString(m_IncomingHeader->GetDeviceType())).arg(QString::fromStdString(e.what()));
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
        QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - Message suggests there should be %2 bytes of data, but only %3 are available. Fragmentation has occurred.").arg(m_MessagePrefix).arg(m_IncomingMessage->GetPackBodySize()).arg(bytesAvailable);

        bytesReceived = in.readRawData(static_cast<char *>(m_IncomingMessage->GetPackBodyPointer()) + m_IncomingMessageBytesReceived, bytesAvailable);
        if (bytesReceived <= 0 || bytesReceived != bytesAvailable)
        {
          m_AbortReading = true;
          QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to read the right size (%2) fragment of OpenIGTLink message data, (bytesReceived=%3).").arg(m_MessagePrefix).arg(bytesAvailable).arg(bytesReceived);
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
        bytesReceived = in.readRawData(static_cast<char *>(m_IncomingMessage->GetPackBodyPointer()) + m_IncomingMessageBytesReceived, bytesRequiredToCompleteMessage);
        if (bytesReceived <= 0)
        {
          m_AbortReading = true;
          QString errorMessage = QObject::tr("%1::OnSocketReadyRead() - Failed to read the right size (%2) OpenIGTLink message, (bytesReceived=%3).").arg(m_MessagePrefix).arg(bytesRequiredToCompleteMessage).arg(bytesReceived);
          QLOG_ERROR() << errorMessage;
          emit SocketError(m_Socket->peerPort(), QAbstractSocket::UnknownSocketError, errorMessage);
          return;
        }
        m_IncomingMessageBytesReceived += bytesReceived;
        bytesAvailable -= bytesReceived;
      }

      // Don't forget to Unpack!
      m_IncomingMessage->Unpack();

      // Check for special case messages. They are squashed here, and not delivered to client.
      igtl::StringMessage::Pointer tmp = dynamic_cast<igtl::StringMessage*>(m_IncomingMessage.GetPointer());
      if (tmp.IsNotNull())
      {
        if (tmp->GetString() == std::string("POKE"))
        {
          QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - Keep-alive received.").arg(m_MessagePrefix);
        }
        else if (tmp->GetString() == std::string("STATS"))
        {
          QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - Stats request received.").arg(m_MessagePrefix);
          this->OnOutputStats();
        }
        // Throw away the message and header
        m_HeaderInProgress = false;
        m_MessageInProgress = false;
        m_IncomingHeader = NULL;
        m_IncomingHeaderTimeStamp = NULL;
        m_IncomingMessage = NULL;
        m_IncomingMessageBytesReceived = 0;
        return;
      }
    } // end if we have a body.

    // Create a new timestamp. Again, this assumes the constructor creates a valid time.
    igtl::TimeStamp::Pointer timeFullyReceived  = igtl::TimeStamp::New();
    timeFullyReceived->GetTime();

    // This is the container we eventually publish.
     NiftyLinkMessageContainer::Pointer msg = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));

    // Set timestamps on NiftyLink container.
    msg->SetTimeArrived(m_IncomingHeaderTimeStamp);
    msg->SetTimeReceived(timeFullyReceived);
    msg->SetMessage(m_IncomingMessage);

    QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - id=%2, class=%3, size=%4 bytes, device='%5'.")
                    .arg(m_MessagePrefix)
                    .arg(msg->GetNiftyLinkMessageId())
                    .arg(m_IncomingMessage->GetNameOfClass())
                    .arg(m_IncomingMessage->GetPackSize())
                    .arg(m_IncomingMessage->GetDeviceType())
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
void NiftyLinkTcpNetworkWorker::OnSend()
{
  // So, this should ONLY be called from the NiftyLinkQThread that this object is bound to.
  NiftyLinkMessageContainer::Pointer message = m_OutboundMessages->GetContainer();
  igtl::MessageBase::Pointer msg = message->GetMessage();

  this->SendMessage(msg);
}



//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSendInternalPing()
{
  // Check if we can avoid sending this message, just to save on network traffic.
  igtl::TimeStamp::Pointer sendStarted = igtl::TimeStamp::New();
  igtlUint64 diff = GetDifferenceInNanoSeconds(sendStarted, m_LastMessageSentTime) / 1000000; // convert nano to milliseconds.
  if (diff < m_KeepAliveInterval/2)
  {
    QLOG_DEBUG() << QObject::tr("%1::OnSendInternalPing() - No real need to send a keep-alive as we have recently processed a message.").arg(objectName());
    return;
  }

  igtl::StringMessage::Pointer msg = igtl::StringMessage::New();
  msg->SetDeviceName("NiftyLinkTcpNetworkWorker");
  msg->SetString("POKE");
  msg->Pack();

  this->SendMessage(msg.GetPointer());
  QLOG_DEBUG() << QObject::tr("%1::OnSendInternalPing() - sent.").arg(m_MessagePrefix);

  emit SentKeepAlive();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::SendMessage(igtl::MessageBase::Pointer msg)
{
  // This doubly double checks we are running in our own thread.
  niftk::NiftyLinkQThread *p = dynamic_cast<niftk::NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  int bytesWritten = m_Socket->write(static_cast<const char*>(msg->GetPackPointer()), msg->GetPackSize());
  if (bytesWritten != msg->GetPackSize())
  {
    QLOG_ERROR() << QObject::tr("%1::SendMessage() - only written %2 bytes instead of %3").arg(m_MessagePrefix).arg(bytesWritten).arg(msg->GetPackSize());
  }

  // Store the time where we last sent a message.
  m_LastMessageSentTime->GetTime();

  QLOG_DEBUG() << QObject::tr("%1::SendMessage() - written %2 bytes.").arg(m_MessagePrefix).arg(bytesWritten);
}

} // end niftk namespace
