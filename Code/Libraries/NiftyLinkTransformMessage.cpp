/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkTransformMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

//-----------------------------------------------------------------------------
NiftyLinkTransformMessage::NiftyLinkTransformMessage(void)
  : NiftyLinkMessage()
{
  m_MessageType = "TRANSFORM";
  m_Message.operator =(NULL);
}


//-----------------------------------------------------------------------------
NiftyLinkTransformMessage::~NiftyLinkTransformMessage(void)
{
  //QLOG_INFO() <<"TransformMessage Destructor" <<m_OwnerName <<m_Id;
}


//-----------------------------------------------------------------------------
NiftyLinkTransformMessage::NiftyLinkTransformMessage(const NiftyLinkTransformMessage &other)
  : NiftyLinkMessage(other)
{
  m_p[0] = other.m_p[0];
  m_p[1] = other.m_p[1];
  m_p[2] = other.m_p[2];
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetMatrix(matrix);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
    return;

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->GetMatrix(matrix);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
QString NiftyLinkTransformMessage::GetMatrixAsString()
{
  if (m_Message.IsNull())
    return QString();

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  std::string stdStr;

  msgPointer->Unpack();
  msgPointer->GetMatrixAsString(stdStr);
  QString strMat = QString(stdStr.c_str());
  msgPointer->Pack();

  return strMat;
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetPosition(float p[3])
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetPosition(p);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetPosition(float p[3])
{
  if (m_Message.IsNull())
    return;

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->GetPosition(p);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetPosition(float px, float py, float pz)
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetPosition(px, py, pz);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetPosition(float &px, float &py, float &pz)
{
  if (m_Message.IsNull())
    return;

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->GetPosition(&px, &py, &pz);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetNormals(float o[3][3])
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetNormals(o);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetNormals(float o[3][3])
{
  if (m_Message.IsNull())
    return;

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->GetNormals(o);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::SetNormals(float t[3], float s[3], float n[3])
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetNormals(t, s, n);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::GetNormals(float t[3], float s[3], float n[3])
{
  if (m_Message.IsNull())
    return;

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->GetNormals(t, s, n);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::InitializeWithTestData(igtl::Matrix4x4& testMatrix)
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();
  msgPointer->SetMatrix(testMatrix);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::InitializeWithRandomData(void)
{
  if (m_Message.IsNull())
    m_Message.operator =(igtl::TransformMessage::New());

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();

  igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  msgPointer->SetMatrix(localMatrix);
  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::Create_GET(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator =(NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::GetTransformMessage::Pointer cmdMsg;
  cmdMsg.operator =(igtl::GetTransformMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->GetTime();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("GET_TRANS");
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::Create_STT(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator =(NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StartTransformMessage::Pointer cmdMsg;
  cmdMsg.operator =(igtl::StartTransformMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->GetTime();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STT_TRANS");
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::Create_STP(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator =(NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StopTransformMessage::Pointer cmdMsg;
  cmdMsg.operator =(igtl::StopTransformMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->GetTime();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STP_TRANS");
}


//-----------------------------------------------------------------------------
void NiftyLinkTransformMessage::Create_RTS(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator =(NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::RTSTransformMessage::Pointer cmdMsg;
  cmdMsg.operator =(igtl::RTSTransformMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->GetTime();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("RTS_TRANS");
}
