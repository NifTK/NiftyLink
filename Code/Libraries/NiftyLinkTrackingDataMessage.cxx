/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkTrackingDataMessage.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "QsLog.h"
#include "QsLogDest.h"

//-----------------------------------------------------------------------------
NiftyLinkTrackingDataMessage::NiftyLinkTrackingDataMessage(void)
  : NiftyLinkMessage()
{
  m_MessageType = "TDATA";
  m_Message.operator = (NULL);
}


//-----------------------------------------------------------------------------
NiftyLinkTrackingDataMessage::~NiftyLinkTrackingDataMessage(void)
{
  //QLOG_INFO() <<"TrackingDataMessage Destructor" <<m_OwnerName <<m_Id;
}


//-----------------------------------------------------------------------------
NiftyLinkTrackingDataMessage::NiftyLinkTrackingDataMessage(const NiftyLinkTrackingDataMessage &other)
  : NiftyLinkMessage(other)
{

  m_p[0] = other.m_p[0];
  m_p[1] = other.m_p[1];
  m_p[2] = other.m_p[2];
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::SetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetMatrix(matrix);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::GetMatrix(igtl::Matrix4x4 &matrix)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return;
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  tElem->GetMatrix(matrix);

  msgPointer->Pack();

}

//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::SetError(float error)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetError(error);

  msgPointer->Pack();
}

//-----------------------------------------------------------------------------
float NiftyLinkTrackingDataMessage::GetError()
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return;
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  float error = tElem->GetError();

  msgPointer->Pack();
  return error;

}




//-----------------------------------------------------------------------------
QString NiftyLinkTrackingDataMessage::GetMatrixAsString()
{
  if (m_Message.IsNull())
  {
    return QString();
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());

  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return QString();
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  igtl::Matrix4x4 matrix;
  tElem->GetMatrix(matrix);

  std::stringstream sstr;
  std::fixed(sstr);
  sstr << std::setprecision(5);

  sstr << "=============" << std::endl;
  sstr << matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << ", " << matrix[0][3] << std::endl;
  sstr << matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << ", " << matrix[1][3] << std::endl;
  sstr << matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << ", " << matrix[2][3] << std::endl;
  sstr << matrix[3][0] << ", " << matrix[3][1] << ", " << matrix[3][2] << ", " << matrix[3][3] << std::endl;
  sstr << "=============" << std::endl;

  QString strMat = QString(sstr.str().c_str());

  msgPointer->Pack();

  return strMat;
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::SetPosition(float p[3])
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetPosition(p);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::GetPosition(float p[3])
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return;
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  tElem->GetPosition(p);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::SetPosition(float px, float py, float pz)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetPosition(px, py, pz);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::GetPosition(float &px, float &py, float &pz)
{
  if (m_Message.IsNull())
  {
    return;
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return;
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  tElem->GetPosition(&px, &py, &pz);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::SetTrackerToolName(QString name)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetName(name.toStdString().c_str());

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
QString NiftyLinkTrackingDataMessage::GetTrackerToolName()
{
  if (m_Message.IsNull())
  {
    return QString();
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return QString();
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  QString str(tElem->GetName());

  msgPointer->Pack();

  return str;
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::SetTrackerToolType(igtlUint8 type)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetType(type);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
igtlUint8 NiftyLinkTrackingDataMessage::GetTrackerToolType()
{
  if (m_Message.IsNull())
  {
    return -1;
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  if (msgPointer->GetNumberOfTrackingDataElements() == 0)
  {
    return -1;
  }

  igtl::TrackingDataElement::Pointer tElem;
  msgPointer->GetTrackingDataElement(0, tElem);
  igtlUint8 type = tElem->GetType();

  msgPointer->Pack();

  return type;
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::InitializeWithTestData(igtl::Matrix4x4& testMatrix)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  tElem->SetMatrix(testMatrix);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::InitializeWithRandomData(void)
{
  if (m_Message.IsNull())
  {
    m_Message.operator = (igtl::TrackingDataMessage::New());
  }

  igtl::TrackingDataMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TrackingDataMessage *>(m_Message.GetPointer());
  msgPointer->Unpack();

  igtl::TrackingDataElement::Pointer tElem;
  int elementNum = msgPointer->GetNumberOfTrackingDataElements();

  if (elementNum == 0)
  {
    tElem = igtl::TrackingDataElement::New();
    msgPointer->AddTrackingDataElement(tElem);
  }
  else
  {
    msgPointer->GetTrackingDataElement(0, tElem);
  }

  igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  tElem->SetMatrix(localMatrix);

  msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::Create_GET(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::GetTrackingDataMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::GetTrackingDataMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("GET_TDATA");
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::Create_STT(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StartTrackingDataMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::StartTrackingDataMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STT_TDATA");
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::Create_STP(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::StopTrackingDataMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::StopTrackingDataMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();

  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("STP_TDATA");
}


//-----------------------------------------------------------------------------
void NiftyLinkTrackingDataMessage::Create_RTS(NiftyLinkMessage::Pointer &msgToCreate)
{
  msgToCreate.operator = (NiftyLinkMessage::Pointer(new NiftyLinkMessage()));

  igtl::RTSTrackingDataMessage::Pointer cmdMsg;
  cmdMsg.operator = (igtl::RTSTrackingDataMessage::New());

  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->Update();

  QString lhn = GetLocalHostAddress();
  cmdMsg->SetTimeStamp(ts);
  cmdMsg->SetDeviceName(lhn.toStdString().c_str());
  cmdMsg->Pack();

  msgToCreate->SetMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
  msgToCreate->ChangeMessageType("RTS_TDATA");
}
