/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkMessageContainer.h"

#include <NiftyLinkUtils.h>

#include <igtl_util.h>

#include "QsLog.h"
#include "QsLogDest.h"

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::NiftyLinkMessageContainer()
: m_Message(NULL)
, m_Id(0)
, m_TimeArrived(0)
, m_TimeReceived(0)
, m_SenderHostName("")
, m_SenderPortNumber(-1)
, m_OwnerName("")
{
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::NiftyLinkMessageContainer(const NiftyLinkMessageContainer& another)
{
  this->ShallowCopy(another);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer& NiftyLinkMessageContainer::operator=(const NiftyLinkMessageContainer& another)
{
  this->ShallowCopy(another);
  return *this;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::ShallowCopy(const NiftyLinkMessageContainer& another)
{
  m_Message = another.m_Message;
  m_Id = another.m_Id;
  m_SenderHostName = another.m_SenderHostName;
  m_SenderPortNumber = another.m_SenderPortNumber;
  m_OwnerName = another.m_OwnerName;
  m_TimeArrived = another.m_TimeArrived;
  m_TimeReceived = another.m_TimeReceived;
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::~NiftyLinkMessageContainer(void)
{
}


//-----------------------------------------------------------------------------
igtlUint64 NiftyLinkMessageContainer::GetNiftyLinkMessageId() const
{
  return m_Id;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetMessage(igtl::MessageBase::Pointer mp)
{
  assert(mp.IsNotNull());
  m_Message = mp;
}


//-----------------------------------------------------------------------------
igtl::MessageBase::Pointer NiftyLinkMessageContainer::GetMessage() const
{
  return m_Message;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetTimeArrived(const igtl::TimeStamp::Pointer& time)
{
  m_TimeArrived = time->GetTimeStampInNanoseconds();
}


//-----------------------------------------------------------------------------
igtlUint64 NiftyLinkMessageContainer::GetTimeArrived() const
{
  return m_TimeArrived;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetTimeReceived(const igtl::TimeStamp::Pointer& time)
{
  m_TimeReceived = time->GetTimeStampInNanoseconds();
}


//-----------------------------------------------------------------------------
igtlUint64 NiftyLinkMessageContainer::GetTimeReceived() const
{
  return m_TimeReceived;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetSenderHostName(const QString &host)
{
  this->m_SenderHostName = host;
}


//-----------------------------------------------------------------------------
QString NiftyLinkMessageContainer::GetSenderHostName() const
{
  return m_SenderHostName;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetSenderPortNumber(const int& portNumber)
{
  this->m_SenderPortNumber = portNumber;
}


//-----------------------------------------------------------------------------
int NiftyLinkMessageContainer::GetSenderPortNumber() const
{
  return this->m_SenderPortNumber;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetOwnerName(const QString& str)
{
  this->m_OwnerName = str;
}


//-----------------------------------------------------------------------------
QString NiftyLinkMessageContainer::GetOwnerName(void)
{
  return this->m_OwnerName;
}


//-----------------------------------------------------------------------------
QString NiftyLinkMessageContainer::GetMessageType()
{
  QString result("");
  if (m_Message.IsNotNull())
  {
    result = QString::fromStdString(m_Message->GetBodyType());
  }
  return result;
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::GetTimeCreated(igtl::TimeStamp::Pointer& time) const
{
  assert(this->m_Message.IsNotNull());
  this->m_Message->GetTimeStamp(time);
}


//-----------------------------------------------------------------------------
igtlInt64 NiftyLinkMessageContainer::GetLatency() const
{
  if (m_Message.IsNull())
  {
    return 0;
  }

  igtlUint32 secCreated;
  igtlUint32 fracCreated;
  this->m_Message->GetTimeStamp(&secCreated, &fracCreated);

  igtlUint64 secCreatedInNano = static_cast<igtlUint64>(secCreated)*1000000000;
  igtlUint64 fracCreatedInNano = igtl_frac_to_nanosec(static_cast<igtlUint32>(fracCreated));
  igtlUint64 timeCreated = secCreatedInNano + fracCreatedInNano;
  igtlInt64 latency = m_TimeReceived - timeCreated; // may be negative if clock shifts.
  return latency;
}

} // end namespace niftk
