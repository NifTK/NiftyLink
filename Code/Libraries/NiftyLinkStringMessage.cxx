/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkStringMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

//-----------------------------------------------------------------------------
NiftyLinkStringMessage::NiftyLinkStringMessage(void)
  : NiftyLinkMessage()
{
  m_MessageType = "STRING";
  m_Message.operator = (NULL);
}


//-----------------------------------------------------------------------------
NiftyLinkStringMessage::~NiftyLinkStringMessage(void)
{
  //QLOG_INFO() <<"StringMessage Destructor" <<m_OwnerName <<m_Id;
}


//-----------------------------------------------------------------------------
NiftyLinkStringMessage::NiftyLinkStringMessage(const NiftyLinkStringMessage &other)
  : NiftyLinkMessage(other)
{
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::SetString(QString string)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StringMessage::New());
  }

  igtl::StringMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StringMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetString(string.toStdString().c_str());
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::SetEncoding(igtlUint16 enc)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StringMessage::New());
  }

  igtl::StringMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StringMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetEncoding(enc);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
QString NiftyLinkStringMessage::GetString()
{
  if (m_Message.IsNull())
  {
    return QString();
  }

  igtl::StringMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StringMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  QString txt(msgPointer->GetString());
  msgPointer->Pack();

  return txt;
}


//-----------------------------------------------------------------------------
igtlUint16 NiftyLinkStringMessage::GetEncoding()
{
  if (m_Message.IsNull())
  {
    return -1;
  }

  igtl::StringMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StringMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  igtlUint16 code = msgPointer->GetEncoding();
  msgPointer->Pack();

  return code;
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::InitializeWithTestData(void)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StringMessage::New());
  }

  igtl::StringMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StringMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetString("This is a test string");
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::Create_GET(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::GetStringMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::GetStringMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("GET_STRING");
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::Create_STT(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StartStringMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::StartStringMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STT_STRING");
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::Create_STP(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StopStringMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::StopStringMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STP_STRING");
}


//-----------------------------------------------------------------------------
void NiftyLinkStringMessage::Create_RTS(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::RTSStringMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::RTSStringMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("RTS_STRING");
}
