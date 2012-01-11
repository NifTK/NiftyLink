#ifndef OIGTLCONNECTOR_H
#define OIGTLCONNECTOR_H

#include <QtCore>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QSignalSpy>

#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
#include "igtlTransformMessage.h"
#include "igtlPositionMessage.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include "igtlStatusMessage.h"

#include "OIGTLThreadBase.h"

class OIGTLConnectorBase : public QObject
{
	Q_OBJECT

//friend class OIGTLListener;
//friend class OIGTLSender;

public:
	OIGTLConnectorBase(QObject *parent = 0) {}
	~OIGTLConnectorBase(void) {}

signals:

public slots:
	
	virtual void setupListenerOnPort(int portNum) {}
	virtual void startListening(int portNum = 0) {}
	virtual void stopListening(int portNum = 0) {}
	virtual bool isListenerActive(int portNum) {}

	virtual void setupSenderOnUrl(QUrl url) {}
	virtual void startSending(QString msgType, int msgNum, int port = 0) {}
	virtual void stopSending(int port = 0) {}
	virtual bool isSenderActive(int portNum) {}

	virtual void closeConnections(void) {}
	virtual void exitConnector(void) {}
	
protected:
	virtual void processMsg(OIGTLMessage * msg) {}
	virtual	void terminateThreads(void) {}
	
private:
	QList<OIGTLThreadBase *>	 m_listenerThreads;
	QList<OIGTLThreadBase *>	 m_senderThreads;

};

#endif