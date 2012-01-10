#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLMessage::OIGTLMessage(QObject *parent)
: QObject(parent)
{
}

OIGTLMessage::~OIGTLMessage(void)
{
	//if (m_message.IsNotNull())
	//	m_message->Delete();

}


void OIGTLMessage::setMessagePointer(igtl::MessageBase::Pointer mp)
{
	m_message.operator =(mp);
}

igtl::MessageBase::Pointer OIGTLMessage::getMessagePointer(void)
{
	return m_message;
}

void OIGTLMessage::setTimeRecieved(igtl::TimeStamp::Pointer ts)
{
	m_timeRecieved.operator=(ts);
}

igtl::TimeStamp::Pointer OIGTLMessage::getTimeRecieved(void)
{
	return m_timeRecieved;
}

igtl::TimeStamp::Pointer OIGTLMessage::getTimeStamp(void)
{
	igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();
	m_message->GetTimeStamp(ts);
	return ts;
}

void OIGTLMessage::setHostName(QString hname)
{
	m_senderHostName = hname;
}

QString OIGTLMessage::getHostName(void)
{
	return m_senderHostName;
}

void OIGTLMessage::setPort(int port)
{
	m_senderPort = port;
}

int OIGTLMessage::getPort(void)
{
	return m_senderPort;
}

void OIGTLMessage::setMessageType(QString type)
{
	m_messageType = type;
}

QString OIGTLMessage::getMessageType(void)
{
	return m_messageType;
}