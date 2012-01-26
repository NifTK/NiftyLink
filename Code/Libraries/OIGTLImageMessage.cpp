#include "OIGTLImageMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLImageMessage::OIGTLImageMessage(void)
	: OIGTLMessage()
{
	m_messageType = "IMAGE";
	m_message.operator =(igtl::ImageMessage::New());

	update();
}

OIGTLImageMessage::~OIGTLImageMessage(void)
{
}

OIGTLImageMessage::OIGTLImageMessage(const OIGTLImageMessage &other)
	: OIGTLMessage(other)
{
}

void OIGTLImageMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetMatrix(matrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
    msgPointer->GetMatrix(matrix);

	//Pack message data
	msgPointer->Pack();

}

//void OIGTLImageMessage::setPosition(float p[3])
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->SetPosition(p);
//	update();
//
//	//Pack message data
//	msgPointer->Pack();
//}
//
//void OIGTLImageMessage::getPosition(float p[3])
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->GetPosition(p);
//
//	//Pack message data
//	msgPointer->Pack();
//}
//
//void OIGTLImageMessage::setPosition(float px, float py, float pz)
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->SetPosition(px, py, pz);
//	update();
//
//	//Pack message data
//	msgPointer->Pack();
//}
//
//void OIGTLImageMessage::getPosition(float &px, float &py, float &pz)
//{
//	if (m_message.IsNull())
//		return;
//
//	igtl::ImageMessage::Pointer msgPointer;
//	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
//	msgPointer->Unpack();
//
//	msgPointer->GetPosition(&px, &py, &pz);
//
//	//Pack message data
//	msgPointer->Pack();
//}

void OIGTLImageMessage::setNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(o);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(o);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(t, s, n);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::getNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(t, s, n);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::initializeWithTestData(void)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

    msgPointer->SetMatrix(dummyTransformMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		return;

	igtl::ImageMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::ImageMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	igtl::Matrix4x4 localMatrix;
    CreateRandomTransformMatrix(localMatrix);

    msgPointer->SetMatrix(localMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLImageMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}

void OIGTLImageMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}

void OIGTLImageMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}

void OIGTLImageMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSImageMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSImageMessage::New());

    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();
    ts->GetTime();

    QString lhn = getLocalHostAddress();

    //Set parameters
    cmdMsg->SetTimeStamp(ts);
    cmdMsg->SetDeviceName(lhn.toStdString().c_str());

    //Pack message data
    cmdMsg->Pack();

    msgToCreate->setMessagePointer((igtl::MessageBase::Pointer) cmdMsg);
}
