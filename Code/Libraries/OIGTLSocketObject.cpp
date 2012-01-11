#include "OIGTLSocketObject.h"

OIGTLSocketObject::OIGTLSocketObject(QObject *parent)
: QObject(parent)
{
	//qRegisterMetaType<OIGTLMessage *>();

	m_port = -1;
	m_listening = false;
	m_connectedToRemote = false;
	
	m_clientConnected = false;
	m_ableToSend = false;

	m_initialized = false;

	m_sender = NULL;
	m_listener = NULL;
	m_mutex = NULL;
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
		disconnect(m_listener, SIGNAL(messageRecieved(OIGTLMessage *)), this, SIGNAL(messageRecieved(OIGTLMessage *)) );

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
	qRegisterMetaType<OIGTLMessage *>();

	m_mutex = new QMutex();
	m_sender = new OIGTLSenderThread(this);
	m_listener = new OIGTLListenerThread(this);

	m_sender->setMutex(m_mutex);
	m_listener->setMutex(m_mutex);

	bool ok;

	ok =  connect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()) );
	ok &= connect(m_sender, SIGNAL(disconnectedFromRemote()), this, SLOT(disconnectedFromRemote()) );
	ok &= connect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()) );

	ok &= connect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()) );
	ok &= connect(m_listener, SIGNAL(clientDisconnected()), this, SLOT(clientDisconnected()) );
	//connect(m_listener, SIGNAL(messageReceived(OIGTLMessage *)), this, SIGNAL(messageReceived(OIGTLMessage *)) );

	// TEST STUFF
	
	ok &= connect(m_listener, SIGNAL(messageReceived(OIGTLMessage *)), this, SLOT(catchMsgSignal(OIGTLMessage *)), Qt::BlockingQueuedConnection);

	ok &= connect(m_listener, SIGNAL(testSignal()), this, SLOT(catchTestSignal( )), Qt::BlockingQueuedConnection);
	
	
	ok &= connect(this, SIGNAL(testSignal()), this, SLOT(catchTestSignal( )));

	m_spy = new QSignalSpy(m_listener, SIGNAL(messageReceived(OIGTLMessage *)));

	ok &= m_spy->isValid();

	if (m_mutex != NULL && m_sender != NULL && m_listener != NULL && ok)
		m_initialized = true;

	//m_spy = new QSignalSpy(m_listener, SIGNAL(testSignal( )));
}

bool OIGTLSocketObject::listenOnPort(int port)
{
	if (!m_initialized)
		initThreads();

	if (m_listener != NULL && m_initialized)
	{
		bool ok = m_listener->initialize(port);

		if (ok)
		{
			m_port = port;
			m_listening = true;
			m_listener->startThread();

			//emit testSignal();
			
			return true;
		}
		else return false;
	}
	else return false;
}

bool OIGTLSocketObject::connectToRemote(QUrl url)
{
	if (!m_initialized)
		initThreads();

	if (m_sender != NULL  && m_initialized)
	{
		char * address = NULL;
		
		if (validateIp(url.host()) == true)
		{
			address = const_cast<char *>(url.host().toStdString().c_str());
		}
		else
		{
			QString ip = resolveHostName(url.host());
			
			if (validateIp(ip))
				address = const_cast<char *>(ip.toStdString().c_str());
			else
				return false;
		}

		int port = url.port();
		
		bool ok = m_sender->initialize(address, port);
		
		if (ok)
		{
			m_port = port;
			//m_connectedToRemote = true;
			return true;
		}
		else return false;
		
	}
	else return false;
}

void OIGTLSocketObject::closeSocket(void)
{
	if (m_sender != NULL)
		m_sender->stopThread();

	if (m_listener != NULL)
		m_listener->stopThread();
}

void OIGTLSocketObject::sendMessage(OIGTLMessage * msg)
{
	if (m_sender != NULL && m_sender->isInitialized())
		m_sender->sendMsg(msg);
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

void OIGTLSocketObject::catchMsgSignal(OIGTLMessage * msg)
{
	qDebug() <<"OIGTLSocketObject::catchMsgSignal : Message signal received...." <<endl;
	if (msg!=NULL)
	{
		emit messageReceived(msg);
	}
}

void OIGTLSocketObject::catchTestSignal()
{
	qDebug() <<"OIGTLSocketObject::catchTestSignal : Test signal received...." <<endl;
}