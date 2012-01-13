#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLMessage::OIGTLMessage(void)
{
	m_timeCreated = igtl::TimeStamp::New();
	m_timeCreated->GetTime();

	m_id = m_timeCreated->GetTimeStampUint64();
}

OIGTLMessage::~OIGTLMessage(void)
{
}

OIGTLMessage::OIGTLMessage(const OIGTLMessage &other)
	: QSharedData(other)
{

	m_message.operator=(other.m_message);
	m_timeRecieved.operator=(other.m_timeRecieved);

	m_senderHostName = other.m_senderHostName;
	m_messageType = other.m_messageType;
	m_senderPort = other.m_senderPort;

	m_id = other.m_id;
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

igtl::TimeStamp::Pointer OIGTLMessage::getTimeCreated(void)
{
	return m_timeCreated;
}

igtl::TimeStamp::Pointer OIGTLMessage::getIGTTimeStamp(void)
{
	igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();
	m_message->GetTimeStamp(ts);
	return ts;
}

igtlUint64 OIGTLMessage::getId(void)
{
	return m_id;
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