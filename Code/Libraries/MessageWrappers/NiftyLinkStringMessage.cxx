/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkStringMessage.h"
#include <NiftyLinkUtils.h>

#include "QsLog.h"
#include "QsLogDest.h"

namespace niftk
{

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetString(string.toStdString().c_str());
  //msgPointer->Pack();
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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetEncoding(enc);
  //msgPointer->Pack();
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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  QString txt(msgPointer->GetString());
  //msgPointer->Pack();

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  igtlUint16 code = msgPointer->GetEncoding();
  //msgPointer->Pack();

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

  if (m_IsPacked)
  {
    msgPointer->Unpack();
    m_IsPacked = false;
  }
  msgPointer->SetString("This is a test string");
  //msgPointer->Pack();
}

} // end namespace niftk
