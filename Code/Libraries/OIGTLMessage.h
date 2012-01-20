#ifndef OIGTLMESSAGE_H
#define OIGTLMESSAGE_H

#include <QtCore>
#include <QObject>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>

#include "igtlBindMessage.h"
//#include "igtlColorTableMessage.h"
#include "igtlImageMessage.h"
//#include "igtlImageMetaMessage.h"
//#include "igtlLabelMetaMessage.h"
#include "igtlPointMessage.h"
#include "igtlPositionMessage.h"
#include "igtlQuaternionTrackingDataMessage.h"
//#include "igtlSensorMessage.h"
#include "igtlStatusMessage.h"
#include "igtlStringMessage.h"
#include "igtlTrackingDataMessage.h"
#include "igtlTrajectoryMessage.h"
#include "igtlTransformMessage.h"

#include "igtlOSUtil.h"
#include "igtlTimeStamp.h"

#include "NiftyLinkCommonWin32ExportHeader.h"

class NIFTYLINKCOMMON_WINEXPORT OIGTLMessage : public QSharedData
{
//	Q_OBJECT

public:
	OIGTLMessage(void);
	~OIGTLMessage(void);

	//Functions that are common in all message types
	typedef OIGTLMessage				              Self;
	typedef QExplicitlySharedDataPointer<Self>        Pointer;
	typedef QExplicitlySharedDataPointer<const Self>  ConstPointer;

	OIGTLMessage(const OIGTLMessage &other); 

	void setMessagePointer(igtl::MessageBase::Pointer mp);
	void getMessagePointer(igtl::MessageBase::Pointer &mp);

	void changeHostName(QString hname);
	QString getHostName(void);

	void changeMessageType(QString type);
	QString getMessageType(void);

	void setPort(int port);
	int getPort(void);

	void setTimeReceived(igtl::TimeStamp::Pointer ts);
	igtl::TimeStamp::Pointer getTimeReceived(void);
	igtl::TimeStamp::Pointer getTimeCreated(void);
	//igtl::TimeStamp::Pointer getIGTTimeStamp(void);

	igtlUint64 getId(void);

	//STT related functions
	void setResolution(igtlUint64 res);
	void getResolution(igtlUint64 &res);

	virtual void initializeWithTestData(void) {}
	virtual void initializeRandomData(void) {}

protected:

	void update();
	QString						m_messageType;
	
	igtl::MessageBase::Pointer  m_message;
	igtl::TimeStamp::Pointer	m_timeReceived;
	igtl::TimeStamp::Pointer	m_timeCreated;

	QString						m_senderHostName;
	
	int							m_senderPort;

	igtlUint64					m_id;
	igtlUint64					m_resolution;


};

Q_DECLARE_METATYPE(OIGTLMessage::Pointer);

#endif