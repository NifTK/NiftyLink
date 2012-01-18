#include "OIGTLSocketObject.h"

OIGTLSocketObject::OIGTLSocketObject(QObject *parent)
: QObject(parent)
{
	m_port = -1;
	m_listening = false;
	m_connectedToRemote = false;
	
	m_clientConnected = false;
	m_ableToSend = false;

	m_initialized = false;

	m_sender = NULL;
	m_listener = NULL;
	m_mutex = NULL;

	initThreads();
}

OIGTLSocketObject::~OIGTLSocketObject(void)
{
	this->closeSocket();

	if (m_sender != NULL)
	{
		disconnect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()) );
		disconnect(m_sender, SIGNAL(disconnectedFromRemote()), this, SLOT(disconnectedFromRemote()) );
		disconnect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()) );

		delete m_sender;
		m_sender = NULL;
	}

	if (m_listener != NULL)
	{
		disconnect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()) );
		disconnect(m_listener, SIGNAL(clientDisconnected()), this, SLOT(clientDisconnected()) );
		disconnect(m_listener, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SIGNAL(messageReceived(OIGTLMessage::Pointer)) );

		delete m_listener;
		m_listener = NULL;
	}

	if (m_mutex != NULL)
	{
		delete m_mutex;
		m_mutex = NULL;
	}
}

void OIGTLSocketObject::initThreads()
{
	qRegisterMetaType<OIGTLMessage::Pointer>();

	m_mutex = new QMutex();
	m_sender = new OIGTLSenderThread(this);
	m_listener = new OIGTLListenerThread(this);

	m_sender->setMutex(m_mutex);
	m_listener->setMutex(m_mutex);

	bool ok;

	ok =  connect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()), Qt::QueuedConnection);
	ok &= connect(m_sender, SIGNAL(disconnectedFromRemote()), this, SLOT(disconnectedFromRemote()), Qt::QueuedConnection);
	ok &= connect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()), Qt::QueuedConnection);
    ok &= connect(this, SIGNAL(messageToSend(OIGTLMessage::Pointer)), m_sender, SLOT(sendMsg(OIGTLMessage::Pointer)));

	ok &= connect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()), Qt::QueuedConnection);
	ok &= connect(m_listener, SIGNAL(clientDisconnected()), this, SLOT(clientDisconnected()), Qt::QueuedConnection);
    ok &= connect(m_listener, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SIGNAL(messageReceived(OIGTLMessage::Pointer)), Qt::QueuedConnection);

	//ok &= connect(m_listener, SIGNAL(messageReceived(OIGTLMessage::Pointer)), this, SLOT(catchMsgSignal(OIGTLMessage::Pointer )), Qt::QueuedConnection);
	//ok &= connect(m_listener, SIGNAL(testSignal( )), this, SLOT(catchTestSignal()), Qt::QueuedConnection);

	if (m_mutex != NULL && m_sender != NULL && m_listener != NULL && ok)
		m_initialized = true;
}

bool OIGTLSocketObject::listenOnPort(int port)
{
	if (!m_initialized)
		return false;

	if (m_listener != NULL && m_initialized)
	{
		m_listener->setObjectName(this->objectName().append("_L"));
		bool ok = m_listener->initialize(port);

		if (ok)
		{
			m_port = port;
			m_listening = true;
			m_listener->startThread();

			QLOG_INFO() <<objectName() <<": " <<"Threads successfully initialized";

			return true;
		}
		else return false;
	}
	else return false;
}

bool OIGTLSocketObject::connectToRemote(QUrl url)
{
	if (!m_initialized)
		return false;

	if (m_sender != NULL  && m_initialized)
	{
		m_sender->setObjectName(this->objectName().append("_S"));

		std::string address_str;
		address_str.clear();
		
		if (validateIp(url.host()) == true)
		{
			address_str = url.host().toStdString();
		}
		else
		{
			QString ip = resolveHostName(url.host());
			
			if (validateIp(ip))
				address_str = url.host().toStdString();
			else
				return false;
		}

		int port = url.port();
		bool ok = m_sender->initialize(address_str, port);
		
		if (ok)
		{
			m_port = port;
			//m_connectedToRemote = true;

			QLOG_INFO() <<objectName() <<": " <<"Threads successfully initialized";
			
			QLOG_INFO() <<objectName() <<": " <<"Connecting to: " <<address_str.c_str() <<" : " <<port <<endl;
			
			return true;
		}
		else return false;
		
	}
	else return false;
}

void OIGTLSocketObject::closeSocket(void)
{
	if (m_sender != NULL)
	{
		m_sender->stopThread();
		
		while (m_sender->isRunning());
	}

	if (m_listener != NULL)
	{
		m_listener->stopThread();
		while (m_listener->isRunning());
	}

	m_port = -1;
	m_listening = false;
	m_connectedToRemote = false;
	
	m_clientConnected = false;
	m_ableToSend = false;

	m_initialized = false;

	QLOG_INFO() <<objectName() <<": " <<"Closing socket, threads terminated.";
}

void OIGTLSocketObject::sendMessage(OIGTLMessage::Pointer msg)
{
	if (m_sender != NULL && m_sender->isInitialized())
        emit messageToSend(msg);

    QLOG_INFO() <<"Emitting.........";
        //m_sender->sendMsg(msg);
}


//*********************************************************
//                    INTERNAL SLOTS
//*********************************************************

void OIGTLSocketObject::connectedToRemote(void)
{
	//Sender established connection to remote host, need to set up listener on socket
	if (m_sender != NULL && m_sender->isInitialized())
	{
		m_connectedToRemote = true;
		m_ableToSend = true;

		m_listener->setObjectName(this->objectName().append("_L"));
		if (m_listener->initialize(m_sender->getSocketPointer()) == true)
			m_listener->startThread();
	}
}

void OIGTLSocketObject::disconnectedFromRemote(void)
{
	if (m_sender != NULL)
		m_sender->stopThread();

	if (m_listener != NULL)
		m_listener->stopThread();

	m_connectedToRemote = false;
	m_ableToSend = false;

	emit lostConnectionToRemote();
}

void OIGTLSocketObject::clientConnected(void)
{
	if (m_listener != NULL && m_listener->isInitialized())
	{
		m_sender->setObjectName(this->objectName().append("_S"));
		if (m_sender->initialize(m_listener->getSocketPointer()) == true)
			m_ableToSend = true;
			//m_sender->startThread();
	}
}

void OIGTLSocketObject::clientDisconnected(void)
{
	if (m_sender != NULL)
	{
		m_sender->stopThread();
		m_ableToSend = false;
	}
}

void OIGTLSocketObject::catchMsgSignal(OIGTLMessage::Pointer msg)
{
	QLOG_INFO() <<objectName() <<": " <<"Message signal received";
	//qDebug() <<"OIGTLSocketObject::catchMsgSignal : Message signal received...." <<endl;
	if (msg.operator !=(NULL))
	{
		emit messageReceived(msg);
	}
}

void OIGTLSocketObject::catchTestSignal()
{
	emit testSignal();
	QLOG_INFO() <<objectName() <<": " <<"Test signal received";
	//qDebug() <<"OIGTLSocketObject::catchTestSignal : Test signal received...." <<endl;
}
