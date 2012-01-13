#ifndef OIGTLSENDERTHREAD_H
#define OIGTLSENDERTHREAD_H

#include "OIGTLThreadBase.h"

class OIGTLSenderThread : public OIGTLThreadBase
{
	Q_OBJECT

friend class OIGTLSocketObject;

signals:
	void connectedToRemote(void);
	void disconnectedFromRemote(void);
	void sendingFinished(void);

protected:
	OIGTLSenderThread(QObject *parent = 0);
	~OIGTLSenderThread(void);
	
	virtual void run(void);

	virtual bool initialize(igtl::Socket::Pointer socket = 0);
	bool initialize(char* hostname, int port = 0);

	//virtual igtl::Socket::Pointer getSocketPointer(void) { return m_extSocket; }

protected slots:
	virtual void sendMsg(OIGTLMessage::Pointer);

	virtual void startThread(void);
	virtual void stopThread(void);

private:
	virtual bool activate(void);

private:
	//igtl::ClientSocket::Pointer m_clientSocket;
	bool m_sendingOnSocket;

	QList<OIGTLMessage::Pointer> m_sendQue;
	QMutex m_queueMutex;
};

#endif