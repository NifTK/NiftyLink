/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkStatusMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

//-----------------------------------------------------------------------------
NiftyLinkStatusMessage::NiftyLinkStatusMessage(void)
  : NiftyLinkMessage()
{
  m_MessageType = "STATUS";
  m_Message.operator = (NULL);
}


//-----------------------------------------------------------------------------
NiftyLinkStatusMessage::~NiftyLinkStatusMessage(void)
{
  //QLOG_INFO() <<"StatusMessage Destructor" <<m_OwnerName <<m_Id;
}


//-----------------------------------------------------------------------------
NiftyLinkStatusMessage::NiftyLinkStatusMessage(const NiftyLinkStatusMessage &other)
  : NiftyLinkMessage(other)
{
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::SetErrorCode(int code)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StatusMessage::New());
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetCode(code);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
int NiftyLinkStatusMessage::GetErrorCode()
{
  if (m_Message.IsNull())
  {
    return -1;
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  int code = msgPointer->GetCode();
  msgPointer->Pack();

  return code;
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::SetErrorSubCode(igtlInt64 subcode)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StatusMessage::New());
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetSubCode(subcode);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
igtlInt64 NiftyLinkStatusMessage::GetErrorSubCode()
{
  if (m_Message.IsNull())
  {
    return -1;
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  igtlInt64 code = msgPointer->GetSubCode();
  msgPointer->Pack();

  return code;
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::SetErrorName(QString name)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StatusMessage::New());
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetErrorName(name.toStdString().c_str());
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
QString NiftyLinkStatusMessage::GetErrorName()
{
  if (m_Message.IsNull())
  {
    return QString();
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  QString txt(msgPointer->GetErrorName());
  msgPointer->Pack();

  return txt;
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::SetStatusString(QString str)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StatusMessage::New());
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetStatusString(str.toStdString().c_str());
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
QString NiftyLinkStatusMessage::GetStatusString()
{
  if (m_Message.IsNull())
  {
    return QString();
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  QString txt(msgPointer->GetStatusString());
  msgPointer->Pack();

  return txt;
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::InitializeWithTestData(void)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::StatusMessage::New());
  }

  igtl::StatusMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::StatusMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetCode(igtl::StatusMessage::STATUS_OK);
  msgPointer->SetSubCode(igtl::StatusMessage::STATUS_PANICK_MODE);
  msgPointer->SetErrorName("ErrorName");
  msgPointer->SetStatusString("StatusString");
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::Create_GET(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::GetStatusMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::GetStatusMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("GET_STATUS");
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::Create_STT(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StartStatusMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::StartStatusMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STT_STATUS");
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::Create_STP(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StopStatusMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::StopStatusMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STP_STATUS");
}


//-----------------------------------------------------------------------------
void NiftyLinkStatusMessage::Create_RTS(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::RTSStatusMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::RTSStatusMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("RTS_STATUS");
}
