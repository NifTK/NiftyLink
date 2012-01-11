#ifndef OIGTLSOCKETOBJECT_H
#define OIGTLSOCKETOBJECT_H

#include "OIGTLSenderThread.h"
#include "OIGTLListenerThread.h"

#include "NiftyLinkCommonWin32ExportHeader.h"

class NIFTYLINKCOMMON_WINEXPORT OIGTLSocketObject : public QObject
{
	Q_OBJECT

signals:
	void messageReceived(OIGTLMessage *msg);
	void sendingFinished();
	void lostConnectionToRemote(void);

	void testSignal(void);

public:
	OIGTLSocketObject(QObject *parent = 0);
	~OIGTLSocketObject(void);

	bool listenOnPort(int port);
	bool connectToRemote(QUrl url);

	void closeSocket(void);

	inline int getPort() { return m_port; }
	inline bool isListening() { return m_listening; }
	inline bool isConnected() { return m_connectedToRemote; }
	inline bool isClientConnecting() { return m_clientConnected; }
	inline bool isAbleToSend() { return m_ableToSend; }

	inline int getTestSignalCalls() {return m_spy->count(); }

public slots:
	void sendMessage(OIGTLMessage * msg);
	void catchMsgSignal(OIGTLMessage * msg);
	void catchTestSignal();

private:
	void initThreads(void);

private slots:
	
	void connectedToRemote(void);
	void disconnectedFromRemote(void);

	void clientConnected(void);
	void clientDisconnected(void);



private:
	int      m_port;
	QMutex * m_mutex;
	
	OIGTLSenderThread   * m_sender;
	OIGTLListenerThread * m_listener;

    bool m_initialized;

	bool m_listening;
	bool m_connectedToRemote;
	
	bool m_clientConnected;
	bool m_ableToSend;

	QSignalSpy * m_spy;

};

#endif