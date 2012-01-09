#ifndef OIGTLTHREADBASE_H
#define OIGTLTHREADBASE_H

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
#include "igtlPointMessage.h"
#include "igtlStringMessage.h"
#include "igtlBindMessage.h"

#include "OIGTLMessage.h"

class OIGTLThreadBase : public QThread
{
	Q_OBJECT

friend class OIGTLConnectorBase;

public:
	OIGTLThreadBase(QObject *parent = 0) {}
	~OIGTLThreadBase(void) {}

signals:
	void requestListenerSetup();
	void socketDisconnected();
	void finishedSending();
	void abcd();

protected:
	void run(void) {}
	
	QMutex * getMutex() {}
	void setMutex(QMutex * mutex) {}
};

#endif