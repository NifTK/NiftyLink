#ifndef OIGTLMESSAGE_H
#define OIGTLMESSAGE_H

#include <QtCore>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

#include "igtlBindMessage.h"
//#include "igtlColorTableMessage.h"
#include "igtlImageMessage.h"
//#include "igtlImageMetaMessage.h"
//#include "igtlLabelMetaMessage.h"
#include "igtlPointMessage.h"
#include "igtlPositionMessage.h"
//#include "igtlQuaternionTrackingDataMessage.h"
//#include "igtlSensorMessage.h"
#include "igtlStatusMessage.h"
#include "igtlStringMessage.h"
//#include "igtlTrackingDataMessage.h"
//#include "igtlTrajectoryMessage.h"
#include "igtlTransformMessage.h"

#include "igtlOSUtil.h"
#include "igtlTimeStamp.h"

#include "NiftyLinkCommonWin32ExportHeader.h"

class NIFTYLINKCOMMON_WINEXPORT OIGTLMessage : public QObject
{
	Q_OBJECT

public:
	OIGTLMessage(QObject *parent = 0);
	~OIGTLMessage(void);

	void setMessagePointer(igtl::MessageBase::Pointer mp);
	igtl::MessageBase::Pointer getMessagePointer(void);

	void setHostName(QString hname);
	QString getHostName(void);

	void setMessageType(QString type);
	QString getMessageType(void);

	void setPort(int port);
	int getPort(void);

	void setTimeRecieved(igtl::TimeStamp::Pointer ts);
	igtl::TimeStamp::Pointer getTimeRecieved(void);
	igtl::TimeStamp::Pointer getTimeStamp(void);

public slots:

signals:

protected:

private:
	igtl::MessageBase::Pointer  m_message;
	igtl::TimeStamp::Pointer	m_timeRecieved;

	QString						m_sender;
	QTime						m_receiveStamp;
	QString						m_senderHostName;
	int							m_senderPort;
	QString						m_messageType;

};

Q_DECLARE_METATYPE(OIGTLMessage * );

#endif