/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

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

//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::NiftyLinkMessageContainer()
  : m_Message(NULL)
, m_Id(0)
, m_SenderHostName("")
, m_SenderPortNumber(-1)
, m_OwnerName("")
{
  m_TimeArrived = igtl::TimeStamp::New();
  m_TimeReceived = igtl::TimeStamp::New();
  m_Id = m_TimeReceived->GetTimeStampInNanoseconds();
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
void NiftyLinkMessageContainer::TouchMessage(const QString &who, const igtlUint64 &when)
{
  this->m_AccessTimes[who] = when;
}


//-----------------------------------------------------------------------------
igtlUint64 NiftyLinkMessageContainer::GetTimeCreated() const
{
  assert(this->m_Message.IsNotNull());

  igtl::TimeStamp::Pointer tmp = igtl::TimeStamp::New();
  this->m_Message->GetTimeStamp(tmp);

  return tmp->GetTimeStampInNanoseconds();
}


//-----------------------------------------------------------------------------
QStringList NiftyLinkMessageContainer::GetAccessTimes()
{
  assert(this->m_Message.IsNotNull());

  QStringList result;
  std::map<QString, igtlUint64>::iterator it;

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  this->m_Message->GetTimeStamp(timeCreated);

  result << QObject::tr("Message:%1, created:%2.%3")
            .arg(m_Id)
            .arg(timeCreated->GetSecond())
            .arg(timeCreated->GetNanosecond());

  for (it = m_AccessTimes.begin(); it != m_AccessTimes.end(); ++it)
  {
    igtl::TimeStamp::Pointer tmp = igtl::TimeStamp::New();
    tmp->SetTimeInNanoseconds(it->second);

    result << QObject::tr("  Accessed:%1.%2, by %3, lag=%4")
              .arg(tmp->GetSecond())
              .arg(tmp->GetNanosecond())
              .arg(it->first)
              .arg(GetDifferenceInNanoSeconds(timeCreated, tmp));
  }

  return result;
}
