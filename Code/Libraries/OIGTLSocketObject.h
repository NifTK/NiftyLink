#ifndef OIGTLSOCKETOBJECT_H
#define OIGTLSOCKETOBJECT_H

#include "OIGTLSenderThread.h"
#include "OIGTLListenerThread.h"

class OIGTLSocketObject : public QObject
{
	Q_OBJECT

signals:
	void messageRecieved(OIGTLMessage *msg);
	void sendingFinished();
	void lostConnectionToRemote(void);

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

public slots:
	void sendMessage(OIGTLMessage * msg);

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

	bool m_listening;
	bool m_connectedToRemote;
	
	bool m_clientConnected;
	bool m_ableToSend;

};

#endif