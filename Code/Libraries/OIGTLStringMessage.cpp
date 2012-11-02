/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "OIGTLStringMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLStringMessage::OIGTLStringMessage(void)
	: OIGTLMessage()
{
	m_messageType = "STRING";
	m_message.operator =(NULL);
}

OIGTLStringMessage::~OIGTLStringMessage(void)
{
  //QLOG_INFO() <<"StringMessage Destructor" <<m_ownerName <<m_id;
}

OIGTLStringMessage::OIGTLStringMessage(const OIGTLStringMessage &other)
	: OIGTLMessage(other)
{
}

void OIGTLStringMessage::setString(QString string)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::StringMessage::New());

	igtl::StringMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StringMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  msgPointer->SetString(string.toStdString().c_str());

	//Pack message data
	msgPointer->Pack();
}

void OIGTLStringMessage::setEncoding(igtlUint16 enc)
{
  if (m_message.IsNull())
		m_message.operator =(igtl::StringMessage::New());

	igtl::StringMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StringMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

	msgPointer->SetEncoding(enc);

	//Pack message data
	msgPointer->Pack();
}

QString OIGTLStringMessage::getString()
{
  if (m_message.IsNull())
		return QString();

	igtl::StringMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StringMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  QString txt(msgPointer->GetString());

	//Pack message data
	msgPointer->Pack();

  return txt;
}

igtlUint16 OIGTLStringMessage::getEncoding()
{
  if (m_message.IsNull())
		return -1;

	igtl::StringMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StringMessage *>(m_message.GetPointer());

	msgPointer->Unpack();

  igtlUint16 code = msgPointer->GetEncoding();

	//Pack message data
	msgPointer->Pack();

  return code;
}

void OIGTLStringMessage::initializeWithTestData(void)
{
	if (m_message.IsNull())
		m_message.operator =(igtl::StringMessage::New());

	igtl::StringMessage::Pointer msgPointer;
	msgPointer = static_cast<igtl::StringMessage *>(m_message.GetPointer());
	msgPointer->Unpack();

  msgPointer->SetString("This is a test string");

	//Pack message data
	msgPointer->Pack();
}

void OIGTLStringMessage::Create_GET(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::GetStringMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::GetStringMessage::New());

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
    msgToCreate->changeMessageType("GET_STRING");
}

void OIGTLStringMessage::Create_STT(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StartStringMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StartStringMessage::New());

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
    msgToCreate->changeMessageType("STT_STRING");
}

void OIGTLStringMessage::Create_STP(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::StopStringMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::StopStringMessage::New());

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
    msgToCreate->changeMessageType("STP_STRING");
}

void OIGTLStringMessage::Create_RTS(OIGTLMessage::Pointer &msgToCreate)
{
    msgToCreate.operator =(OIGTLMessage::Pointer(new OIGTLMessage()));

    igtl::RTSStringMessage::Pointer cmdMsg;
    cmdMsg.operator =(igtl::RTSStringMessage::New());

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
    msgToCreate->changeMessageType("RTS_STRING");
}
