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
NiftyLinkTcpNetworkWorker::NiftyLinkTcpNetworkWorker(QTcpSocket *socket, QObject *parent)
: QObject(parent)
, m_Socket(socket)
, m_HeaderInProgress(false)
, m_MessageInProgress(false)
, m_IncomingHeader(NULL)
, m_IncomingHeaderTimeStamp(NULL)
, m_IncomingMessage(NULL)
, m_IncomingMessageBytesReceived(0)
, m_AbortReading(false)
{
  assert(m_Socket);

  qRegisterMetaType<niftk::NiftyLinkMessageContainer::Pointer>("niftk::NiftyLinkMessageContainer::Pointer");
  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

  m_MessagePrefix = QObject::tr("NiftyLinkTcpNetworkWorker(%1)").arg(m_Socket->socketDescriptor());
  this->setObjectName(m_MessagePrefix);

  connect(this, SIGNAL(InternalSendSignal(igtl::MessageBase::Pointer)), SLOT(OnSend(igtl::MessageBase::Pointer)));
  connect(m_Socket, SIGNAL(bytesWritten(qint64)), this, SIGNAL(BytesSent(qint64)));
  connect(m_Socket, SIGNAL(disconnected()), this, SLOT(OnSocketDisconnected()));
  connect(m_Socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));
  connect(m_Socket, SIGNAL(readyRead()), this, SLOT(OnSocketReadyRead()));

  QLOG_INFO() << QObject::tr("%1::NiftyLinkTcpNetworkWorker() - created.").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
NiftyLinkTcpNetworkWorker::~NiftyLinkTcpNetworkWorker()
{
  QLOG_INFO() << QObject::tr("%1::~NiftyLinkTcpNetworkWorker() - destroyed").arg(m_MessagePrefix);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketDisconnected()
{
  QThread::currentThread()->quit();
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSocketError(QAbstractSocket::SocketError error)
{
  QLOG_INFO() << QObject::tr("%1::OnSocketError(code=%2)").arg(objectName()).arg(error);
  emit SocketError(m_Socket->peerPort(), error);
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
      int bytesReceived = in.readRawData(static_cast<char*>(m_IncomingHeader->GetPackPointer()), m_IncomingHeader->GetPackSize());
      if (bytesReceived <= 0)
      {
        m_AbortReading = true;
        QLOG_ERROR() << QObject::tr("%1::OnSocketReadyRead() - Failed to receive valid OpenIGTLink header. I expected to read a full header, but couldn't. This suggests junk on the wire, or system failure.").arg(m_MessagePrefix);
        emit SocketError(m_Socket->peerPort(), QTcpSocket::UnknownSocketError);
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
        QLOG_ERROR() << QObject::tr("%1::OnSocketReadyRead() - Failed to create message type %2. Error was %3. This suggests junk on the wire, or incorrectly encoded messages, or similar.").arg(m_MessagePrefix).arg(QString::fromStdString(m_IncomingHeader->GetDeviceType())).arg(QString::fromStdString(e.what()));
        emit SocketError(m_Socket->peerPort(), QTcpSocket::UnknownSocketError);
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
        QLOG_WARN() << QObject::tr("%1::OnSocketReadyRead() - Message suggests there should be %2 bytes of data, but only %3 are available. Fragmentation has occurred.").arg(m_MessagePrefix).arg(m_IncomingMessage->GetPackBodySize()).arg(bytesAvailable);

        int bytesReceived = in.readRawData(static_cast<char *>(m_IncomingMessage->GetPackBodyPointer()) + m_IncomingMessageBytesReceived, bytesAvailable);
        if (bytesReceived <= 0)
        {
          m_AbortReading = true;
          QLOG_ERROR() << QObject::tr("%1::OnSocketReadyRead() - Failed to read the right size (%2) fragment of OpenIGTLink message data, (bytesReceived=%3).").arg(m_MessagePrefix).arg(bytesAvailable).arg(bytesReceived);
          emit SocketError(m_Socket->peerPort(), QTcpSocket::UnknownSocketError);
          return;
        }
        m_IncomingMessageBytesReceived += bytesReceived;
        bytesAvailable -= bytesReceived;
        return;
      }
      else
      {
        // Receive remaining data from the socket.
        int bytesReceived = in.readRawData(static_cast<char *>(m_IncomingMessage->GetPackBodyPointer()) + m_IncomingMessageBytesReceived, bytesRequiredToCompleteMessage);
        if (bytesReceived <= 0)
        {
          m_AbortReading = true;
          QLOG_ERROR() << QObject::tr("%1::OnSocketReadyRead() - Failed to read the right size (%2) OpenIGTLink message, (bytesReceived=%3).").arg(m_MessagePrefix).arg(bytesRequiredToCompleteMessage).arg(bytesReceived);
          emit SocketError(m_Socket->peerPort(), QTcpSocket::UnknownSocketError);
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
          QLOG_INFO() << QObject::tr("%1::OnSocketReadyRead() - Keep-alive received, restarting the timeouter.").arg(m_MessagePrefix);
        }
        else if (tmp->GetString() == std::string("STATS"))
        {
          QLOG_INFO() << QObject::tr("%1::OnSocketReadyRead() - Stats request received.").arg(m_MessagePrefix);
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

    // This is the container we eventually publish.
    NiftyLinkMessageContainer::Pointer msg = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));

    // Set timestamps on NiftyLink container.
    msg->SetTimeArrived(m_IncomingHeaderTimeStamp);
    msg->SetTimeReceived(timeFullyReceived);
    msg->SetMessage(m_IncomingMessage);

  //  m_NumberOfMessagesReceived++;

    // In OpenIGTLink paper (Tokuda 2009), Latency is defined as the difference
    // between last byte received and first byte sent.
    igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
    m_IncomingHeader->GetTimeStamp(timeCreated);

    igtlUint64 latency = 0;//niftk::GetDifferenceInNanoSeconds(timeCreated, timeReceived);
  //  m_ListOfLatencies.append(latency);

    QLOG_DEBUG() << QObject::tr("%1::OnSocketReadyRead() - id=%2, class=%3, size=%4 bytes, device='%5', latency=%6, avail=%7.")
                    .arg(m_MessagePrefix)
                    .arg(msg->GetNiftyLinkMessageId())
                    .arg(m_IncomingMessage->GetNameOfClass())
                    .arg(m_IncomingMessage->GetPackSize())
                    .arg(m_IncomingMessage->GetDeviceType())
                    .arg(latency)
                    .arg(bytesAvailable)
                    ;

    // We now reset these variables.
    // We don't reset the smart pointers, as we publish message to rest of application.
    m_HeaderInProgress = false;
    m_MessageInProgress = false;
    m_IncomingMessageBytesReceived = 0;

    emit MessageReceived(m_Socket->peerPort(), msg);

  } while (bytesAvailable > 0);

  return;
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::Send(igtl::MessageBase::Pointer msg)
{
  emit this->InternalSendSignal(msg);
}


//-----------------------------------------------------------------------------
void NiftyLinkTcpNetworkWorker::OnSend(igtl::MessageBase::Pointer msg)
{
  // This doubly double checks we are running in our own thread.
  niftk::NiftyLinkQThread *p = dynamic_cast<niftk::NiftyLinkQThread*>(QThread::currentThread());
  assert(p != NULL);

  // Make sure the message is timestamped and packed.
  igtl::TimeStamp::Pointer sendStarted = igtl::TimeStamp::New();
  msg->SetTimeStamp(sendStarted);
  msg->Pack();

  int bytesWritten = m_Socket->write(static_cast<const char*>(msg->GetPackPointer()), msg->GetPackSize());
  if (bytesWritten != msg->GetPackSize())
  {
    QLOG_ERROR() << QObject::tr("%1::Send() - only written %2 bytes instead of %3").arg(m_MessagePrefix).arg(bytesWritten).arg(msg->GetPackSize());
  }
  QLOG_INFO() << QObject::tr("%1::Send() - written %2 bytes.").arg(m_MessagePrefix).arg(bytesWritten);
}

} // end niftk namespace
