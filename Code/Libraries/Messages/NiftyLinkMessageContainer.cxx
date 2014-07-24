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

#include "QsLog.h"
#include "QsLogDest.h"

#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::NiftyLinkMessageContainer()
  : m_Message(NULL)
, m_Id(0)
, m_SenderHostName("")
, m_SenderPortNumber(-1)
, m_OwnerName("")
{
  m_TimeArrived = igtl::TimeStamp::New();
  m_TimeArrived->GetTime();
  m_TimeReceived = igtl::TimeStamp::New();
  m_TimeReceived->GetTime();
  m_Id = m_TimeReceived->GetTimeStampInNanoseconds();
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
  m_TimeArrived->SetTimeInNanoseconds(another.m_TimeArrived->GetTimeStampInNanoseconds());
  m_TimeReceived->SetTimeInNanoseconds(another.m_TimeReceived->GetTimeStampInNanoseconds());
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
  igtlUint64 tmp = time->GetTimeStampInNanoseconds();
  this->m_TimeArrived->SetTimeInNanoseconds(tmp);
}


//-----------------------------------------------------------------------------
igtlUint64 NiftyLinkMessageContainer::GetTimeArrived() const
{
  return this->m_TimeArrived->GetTimeStampInNanoseconds();
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageContainer::SetTimeReceived(const igtl::TimeStamp::Pointer& time)
{
  igtlUint64 tmp = time->GetTimeStampInNanoseconds();
  this->m_TimeReceived->SetTimeInNanoseconds(tmp);
}


//-----------------------------------------------------------------------------
igtlUint64 NiftyLinkMessageContainer::GetTimeReceived() const
{
  return this->m_TimeReceived->GetTimeStampInNanoseconds();
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
igtlUint64 NiftyLinkMessageContainer::GetTimeCreated() const
{
  assert(this->m_Message.IsNotNull());

  igtl::TimeStamp::Pointer tmp = igtl::TimeStamp::New();
  this->m_Message->GetTimeStamp(tmp);

  return tmp->GetTimeStampInNanoseconds();
}

} // end namespace niftk
