#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"
#include "NiftyLinkUtils.h"

OIGTLMessage::OIGTLMessage(void)
{
	m_timeCreated = igtl::TimeStamp::New();
	m_timeCreated->GetTime();

	m_id = m_timeCreated->GetTimeStampUint64();

	m_id = 0;
	m_resolution = 0;
}

OIGTLMessage::~OIGTLMessage(void)
{
}

OIGTLMessage::OIGTLMessage(const OIGTLMessage &other)
	: QSharedData(other)
{

	m_message.operator=(other.m_message);
	m_timeReceived.operator=(other.m_timeReceived);

	m_senderHostName = other.m_senderHostName;
	m_messageType = other.m_messageType;
	m_senderPort = other.m_senderPort;

	m_resolution = other.m_resolution;
	m_id = other.m_id;
}

void OIGTLMessage::setMessagePointer(igtl::MessageBase::Pointer mp)
{
	m_message.operator =(mp);
	m_messageType = QString(m_message->GetDeviceType());
	m_message->GetTimeStamp(m_timeCreated);
	m_senderHostName = QString(m_message->GetDeviceName());
}

void OIGTLMessage::getMessagePointer(igtl::MessageBase::Pointer &mp)
{
	mp.operator = (m_message);
}

void OIGTLMessage::setTimeReceived(igtl::TimeStamp::Pointer ts)
{
	m_timeReceived.operator=(ts);
}

igtl::TimeStamp::Pointer OIGTLMessage::getTimeReceived(void)
{
	return m_timeReceived;
}

igtl::TimeStamp::Pointer OIGTLMessage::getTimeCreated(void)
{
	return m_timeCreated;
}

igtlUint64 OIGTLMessage::getId(void)
{
	return m_id;
}

void OIGTLMessage::changeHostName(QString hname)
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

void OIGTLMessage::changeMessageType(QString type)
{
	m_messageType = type;
}

QString OIGTLMessage::getMessageType(void)
{
	return m_messageType;
}

void OIGTLMessage::setResolution(igtlUint64 res)
{
	m_resolution = res;

	if (m_message.IsNull())
		return;

	m_message->Unpack();
	
	if (strcmp(m_message->GetNameOfClass(), "igtl::StartTransformMessage") == 0)
	{
		igtl::StartTransformMessage::Pointer pointer = static_cast<igtl::StartTransformMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartTrajectoryMessage") == 0)
	{
		igtl::StartTrajectoryMessage::Pointer pointer = static_cast<igtl::StartTrajectoryMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartTrackingDataMessage") == 0)
	{
		igtl::StartTrackingDataMessage::Pointer pointer = static_cast<igtl::StartTrackingDataMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartStringMessage") == 0)
	{
		igtl::StartStringMessage::Pointer pointer = static_cast<igtl::StartStringMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartStatusMessage") == 0)
	{
		igtl::StartStatusMessage::Pointer pointer = static_cast<igtl::StartStatusMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartQuaternionTrackingDataMessage") == 0)
	{
		igtl::StartQuaternionTrackingDataMessage::Pointer pointer = static_cast<igtl::StartQuaternionTrackingDataMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartPositionMessage") == 0)
	{
		igtl::StartPositionMessage::Pointer pointer = static_cast<igtl::StartPositionMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartPointMessage") == 0)
	{
		igtl::StartPointMessage::Pointer pointer = static_cast<igtl::StartPointMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartImageMessage") == 0)
	{
		igtl::StartImageMessage::Pointer pointer = static_cast<igtl::StartImageMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartBindMessage") == 0)
	{
		igtl::StartBindMessage::Pointer pointer = static_cast<igtl::StartBindMessage *>(m_message.GetPointer());
		pointer->SetResolution(res);
	}
	
	m_message->Pack();
}

void OIGTLMessage::getResolution(igtlUint64 &res)
{
	if (m_message.IsNull())
		return;

	m_message->Unpack();
	
	if (strcmp(m_message->GetNameOfClass(), "igtl::StartTransformMessage") == 0)
	{
		igtl::StartTransformMessage::Pointer pointer = static_cast<igtl::StartTransformMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartTrajectoryMessage") == 0)
	{
		igtl::StartTrajectoryMessage::Pointer pointer = static_cast<igtl::StartTrajectoryMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartTrackingDataMessage") == 0)
	{
		igtl::StartTrackingDataMessage::Pointer pointer = static_cast<igtl::StartTrackingDataMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartStringMessage") == 0)
	{
		igtl::StartStringMessage::Pointer pointer = static_cast<igtl::StartStringMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartStatusMessage") == 0)
	{
		igtl::StartStatusMessage::Pointer pointer = static_cast<igtl::StartStatusMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartQuaternionTrackingDataMessage") == 0)
	{
		igtl::StartQuaternionTrackingDataMessage::Pointer pointer = static_cast<igtl::StartQuaternionTrackingDataMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartPositionMessage") == 0)
	{
		igtl::StartPositionMessage::Pointer pointer = static_cast<igtl::StartPositionMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartPointMessage") == 0)
	{
		igtl::StartPointMessage::Pointer pointer = static_cast<igtl::StartPointMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartImageMessage") == 0)
	{
		igtl::StartImageMessage::Pointer pointer = static_cast<igtl::StartImageMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	else if (strcmp(m_message->GetNameOfClass(), "igtl::StartBindMessage") == 0)
	{
		igtl::StartBindMessage::Pointer pointer = static_cast<igtl::StartBindMessage *>(m_message.GetPointer());
		m_resolution = pointer->GetResolution();
	}
	
	m_message->Pack();

	res = m_resolution;
}

void OIGTLMessage::update()
{
	if (m_message.IsNull())
		return;

	igtl::TimeStamp::Pointer ts;
	ts = igtl::TimeStamp::New();
	ts->GetTime();

	QString lhn = getLocalHostAddress2();

	m_message->SetTimeStamp(ts);
	m_message->SetDeviceName(lhn.toStdString().c_str());

	m_timeCreated.operator =(ts);
	m_senderHostName = lhn;
}
