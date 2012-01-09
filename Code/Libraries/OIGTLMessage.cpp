#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

OIGTLMessage::OIGTLMessage(QObject *parent)
: QObject(parent)
{
}

OIGTLMessage::~OIGTLMessage(void)
{
	if (m_message.IsNotNull())
		m_message->Delete();

	//if (m_socket.IsNotNull())
	//	m_message->Delete();
}


void OIGTLMessage::setMessagePointer(igtl::MessageBase::Pointer mp)
{
	m_message = mp;
}

igtl::MessageBase::Pointer OIGTLMessage::getMessagePointer(void)
{
	return m_message;
}

void OIGTLMessage::setTimeStamp(QTime stamp)
{
	m_receiveStamp = stamp;
}

QTime OIGTLMessage::getTimeStamp(void)
{
	return m_receiveStamp;
}

void OIGTLMessage::setSocketPointer(igtl::Socket::Pointer sp)
{
	m_socket = sp;
}
	
igtl::Socket::Pointer OIGTLMessage::getSocketPointer(void)
{
	return m_socket;
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