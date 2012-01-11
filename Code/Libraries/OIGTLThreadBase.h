#ifndef OIGTLTHREADBASE_H
#define OIGTLTHREADBASE_H

#include <QtCore>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QSignalSpy>

#include "OIGTLMessage.h"
#include "igtlSocket.h"
#include "igtlServerSocket.h"

#include "NiftyLinkUtils.h"
#include "QsLog.h"
#include "QsLogDest.h"

class OIGTLThreadBase : public QThread
{
	Q_OBJECT

public:
	OIGTLThreadBase(QObject *parent = 0);
	~OIGTLThreadBase(void);

protected:
	virtual void run(void) {}
	
	virtual bool initialize(igtl::Socket::Pointer socket = 0) { return false; }
	virtual bool activate(void) { return false; }

	virtual void sendMsg(OIGTLMessage &msg) {}
	virtual igtl::Socket::Pointer getSocketPointer(void) { return m_extSocket; }

	inline  bool isActive() {return m_running; }
	inline  bool isInitialized() {return m_initialized; }
	inline  int  getPort(void) {return m_port; }
	inline  void setMutex(QMutex * mutex) { m_mutex = mutex; }
	
protected slots:
	virtual void startThread() {}
	virtual void stopThread() {}
	//virtual void quitThread() {}

protected:
	int      m_port;
	bool     m_running;
	bool     m_initialized;
    QMutex * m_mutex;
	
	igtl::Socket::Pointer m_extSocket;
};

#endif