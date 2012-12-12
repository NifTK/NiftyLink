/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"
#include "NiftyLinkUtils.h"

OIGTLMessage::OIGTLMessage(void)
{
  m_timeCreated = igtl::TimeStamp::New();
  m_timeCreated->GetTime();
  m_senderPort = -1;

  m_senderHostName = QString("localhost");
  m_id = GetTimeInNanoSeconds(m_timeCreated);
  
  //m_timeCreated = NULL;
  m_resolution = 0;
  m_processed  = 0;
  m_ownerName.append("!");
}

OIGTLMessage::~OIGTLMessage(void)
{
  //QLOG_INFO() <<"Message destructor (Base class): "<<m_ownerName <<m_id;

  m_message.operator =(NULL);
  m_timeReceived.operator =(NULL);
  m_timeCreated.operator =(NULL);
}

OIGTLMessage::OIGTLMessage(const OIGTLMessage &other)
	: QSharedData(other)
{
  // Copy constructor
	m_message.operator     =(other.m_message);
	m_timeReceived.operator=(other.m_timeReceived);

	m_senderHostName       = other.m_senderHostName;
	m_messageType          = other.m_messageType;
	m_senderPort           = other.m_senderPort;

	m_resolution           = other.m_resolution;
	m_id                   = other.m_id;

  m_ownerName            = other.m_ownerName;
}

void OIGTLMessage::setMessagePointer(igtl::MessageBase::Pointer mp)
{
  // Embeds the OpenIGTLink message pointer into the message
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
  if (m_message.IsNull())
    return;
  
  m_senderHostName = hname;
  
  m_message->Unpack(); 

  m_message->SetDeviceName(m_senderHostName.toStdString().c_str());

  m_message->Pack();
}

QString OIGTLMessage::getHostName(void)
{
  if (m_message.IsNull())
    return QString();
 
  m_message->Unpack(); 

  m_senderHostName = m_message->GetDeviceName();

  m_message->Pack();

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
  // This is to set the streaming frequency
	m_resolution = res;

	if (m_message.IsNull())
		return;

  QString className(m_message->GetNameOfClass());

  if (!className.contains("Start"))
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

  QString className(m_message->GetNameOfClass());

  if (!className.contains("Start"))
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

void OIGTLMessage::update(QString hostname, igtl::TimeStamp::Pointer ts)
{
  // Updates timestamp and hostname
	
  if (m_message.IsNull())
    return;

  m_timeCreated.operator =(ts);
  m_senderHostName = hostname;
  
  m_message->Unpack(); 

  m_message->SetTimeStamp(ts);
  m_message->SetDeviceName(m_senderHostName.toStdString().c_str());

  m_message->Pack();
}

void OIGTLMessage::update(QString hostname)
{
  // Updates timestamp and hostname
	
  if (m_message.IsNull())
    return;
  
  igtl::TimeStamp::Pointer ts;
  ts = igtl::TimeStamp::New();
  ts->GetTime();

  m_timeCreated.operator =(ts);
  m_senderHostName = hostname;

  m_id = GetTimeInNanoSeconds(ts);
  
  m_message->Unpack(); 

  m_message->SetTimeStamp(ts);
  m_message->SetDeviceName(m_senderHostName.toStdString().c_str());

  m_message->Pack();
}

void OIGTLMessage::setOwnerName(QString str) 
  { 
    if (!m_ownerName.isEmpty() && !m_ownerName.isNull())
      m_ownerName.clear(); 
    m_ownerName.append(str);
  }

