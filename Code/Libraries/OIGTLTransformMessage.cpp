/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "OIGTLTransformMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

//-----------------------------------------------------------------------------
OIGTLTransformMessage::OIGTLTransformMessage(void)
	: OIGTLMessage()
{
	m_messageType = "TRANSFORM";
	m_message.operator =(NULL);
}


//-----------------------------------------------------------------------------
OIGTLTransformMessage::~OIGTLTransformMessage(void)
{
  //QLOG_INFO() <<"TransformMessage Destructor" <<m_ownerName <<m_id;
}


//-----------------------------------------------------------------------------
OIGTLTransformMessage::OIGTLTransformMessage(const OIGTLTransformMessage &other)
	: OIGTLMessage(other)
{
	m_p[0] = other.m_p[0];
  m_p[1] = other.m_p[1];
  m_p[2] = other.m_p[2];
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetMatrix(matrix);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
  msgPointer->GetMatrix(matrix);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
QString OIGTLTransformMessage::getMatrixAsString()
{
  if (m_message.IsNull())
    return QString();

  igtl::TransformMessage::Pointer msgPointer;
  msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

  std::string stdStr;

  msgPointer->Unpack();
  msgPointer->GetMatrixAsString(stdStr);
  QString strMat = QString(stdStr.c_str());
  msgPointer->Pack();

  return strMat;
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::setPosition(float p[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetPosition(p);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::getPosition(float p[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetPosition(p);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::setPosition(float px, float py, float pz)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetPosition(px, py, pz);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::getPosition(float &px, float &py, float &pz)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetPosition(&px, &py, &pz);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::setNormals(float o[3][3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetNormals(o);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::getNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetNormals(o);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetNormals(t, s, n);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::getNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->GetNormals(t, s, n);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::initializeWithTestData(igtl::Matrix4x4& testMatrix)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
	msgPointer->SetMatrix(testMatrix);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::TransformMessage::New());

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

	igtl::Matrix4x4 localMatrix;
  CreateRandomTransformMatrix(localMatrix);

  msgPointer->SetMatrix(localMatrix);
	msgPointer->Pack();
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetTransformMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
    msgToCreate->changeMessageType("GET_TRANS");
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartTransformMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
    msgToCreate->changeMessageType("STT_TRANS");
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopTransformMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
    msgToCreate->changeMessageType("STP_TRANS");
}


//-----------------------------------------------------------------------------
void OIGTLTransformMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSTransformMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
    msgToCreate->changeMessageType("RTS_TRANS");
}
