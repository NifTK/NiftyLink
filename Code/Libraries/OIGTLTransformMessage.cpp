/*=============================================================================

 NiftyLink:  A software library to facilitate communication over OpenIGTLink.

             http://cmic.cs.ucl.ac.uk/
             http://www.ucl.ac.uk/

 Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

 Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
 Revision          : $Revision: 3300 $
 Last modified by  : $Author: mjc $

 Original author   : m.clarkson@ucl.ac.uk

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 ============================================================================*/

#include "OIGTLTransformMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLTransformMessage::OIGTLTransformMessage(void)
	: OIGTLMessage()
{
	m_messageType = "TRANSFORM";
	m_message.operator =(igtl::TransformMessage::New());

	update();
}

OIGTLTransformMessage::~OIGTLTransformMessage(void)
{
}

OIGTLTransformMessage::OIGTLTransformMessage(const OIGTLTransformMessage &other)
	: OIGTLMessage(other)
{

	m_p[0] = other.m_p[0];
}

void OIGTLTransformMessage::setMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetMatrix(matrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getMatrix(igtl::Matrix4x4 &matrix)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());

	msgPointer->Unpack();
    msgPointer->GetMatrix(matrix);

	//Pack message data
	msgPointer->Pack();

}

void OIGTLTransformMessage::setPosition(float p[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetPosition(p);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getPosition(float p[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetPosition(p);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::setPosition(float px, float py, float pz)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetPosition(px, py, pz);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getPosition(float &px, float &py, float &pz)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetPosition(&px, &py, &pz);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::setNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(o);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getNormals(float o[3][3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(o);

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::setNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetNormals(t, s, n);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::getNormals(float t[3], float s[3], float n[3])
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->GetNormals(t, s, n);
	
	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::initializeWithTestData(void)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

    msgPointer->SetMatrix(dummyTransformMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::initializeWithRandomData(void)
{
	if (m_message.IsNull())
		return;

	igtl::TransformMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::TransformMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	igtl::Matrix4x4 localMatrix;
    CreateRandomTransformMatrix(localMatrix);

    msgPointer->SetMatrix(localMatrix);
	update();

	//Pack message data
	msgPointer->Pack();
}

void OIGTLTransformMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetTransformMessage::New());

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

void OIGTLTransformMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartTransformMessage::New());

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

void OIGTLTransformMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopTransformMessage::New());

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

void OIGTLTransformMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSTransformMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSTransformMessage::New());

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
