#ifndef OIGTLMESSAGE_H
#define OIGTLMESSAGE_H

#include <QtCore>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>


#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
#include "igtlTransformMessage.h"
#include "igtlPositionMessage.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include "igtlStatusMessage.h"

class OIGTLMessage : public QObject
{
	Q_OBJECT

public:
	OIGTLMessage(QObject *parent = 0);
	~OIGTLMessage(void);

	void setMessagePointer(igtl::MessageBase::Pointer mp);
	igtl::MessageBase::Pointer getMessagePointer(void);

	void setSocketPointer(igtl::Socket::Pointer sp);
	igtl::Socket::Pointer getSocketPointer(void);
	
	void setTimeStamp(QTime stamp);
	QTime getTimeStamp(void);

	void setHostName(QString hname);
	QString getHostName(void);

	void setPort(int port);
	int getPort(void);

	void setMessageType(QString type);
	QString getMessageType(void);

public slots:

signals:

protected:

private:
	igtl::MessageBase::Pointer  m_message;
	igtl::Socket::Pointer       m_socket;
	QString						m_sender;
	QTime						m_receiveStamp;
	QString						m_senderHostName;
	int							m_senderPort;
	QString						m_messageType;

};

#endif