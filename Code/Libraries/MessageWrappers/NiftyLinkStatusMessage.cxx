/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkStatusMessage.h"
#include <NiftyLinkUtils.h>

#include "QsLog.h"
#include "QsLogDest.h"

namespace niftk
{

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetCode(code);
  //msgPointer->Pack();
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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  int code = msgPointer->GetCode();
  //msgPointer->Pack();

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetSubCode(subcode);
  //msgPointer->Pack();
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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  igtlInt64 code = msgPointer->GetSubCode();
  //msgPointer->Pack();

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetErrorName(name.toStdString().c_str());
  //msgPointer->Pack();
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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  QString txt(msgPointer->GetErrorName());
  //msgPointer->Pack();

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetStatusString(str.toStdString().c_str());
  //msgPointer->Pack();
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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  QString txt(msgPointer->GetStatusString());
  //msgPointer->Pack();

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetCode(igtl::StatusMessage::STATUS_OK);
  msgPointer->SetSubCode(igtl::StatusMessage::STATUS_PANICK_MODE);
  msgPointer->SetErrorName("ErrorName");
  msgPointer->SetStatusString("StatusString");
  //msgPointer->Pack();
}

} // end namespace niftk
