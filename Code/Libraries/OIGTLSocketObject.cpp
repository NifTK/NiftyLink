#include "OIGTLSocketObject.h"

OIGTLSocketObject::OIGTLSocketObject(QObject *parent)
: QObject(parent)
{
	m_port = -1;
	m_listening = false;
	m_connectedToRemote = false;
	
	m_clientConnected = false;
	m_ableToSend = false;

	m_mutex = new QMutex();
	m_sender = new OIGTLSenderThread(this);
	m_listener = new OIGTLListenerThread(this);

	connect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()) );
	connect(m_sender, SIGNAL(disconnectedFromRemote()), this, SLOT(disconnectedFromRemote()) );
	connect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()) );

	connect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()) );
	connect(m_listener, SIGNAL(clientDisconnected()), this, SLOT(clientDisconnected()) );
	connect(m_listener, SIGNAL(messageRecieved(OIGTLMessage *)), this, SIGNAL(messageRecieved(OIGTLMessage *)) );

	m_sender->setMutex(m_mutex);
	m_listener->setMutex(m_mutex);
}

OIGTLSocketObject::~OIGTLSocketObject(void)
{
	this->closeSocket();

	disconnect(m_sender, SIGNAL(connectedToRemote()), this, SLOT(connectedToRemote()) );
	disconnect(m_sender, SIGNAL(disconnectedFromRemote()), this, SLOT(disconnectedFromRemote()) );
	disconnect(m_sender, SIGNAL(sendingFinished()), this, SIGNAL(sendingFinished()) );

	disconnect(m_listener, SIGNAL(clientConnected()), this, SLOT(clientConnected()) );
	disconnect(m_listener, SIGNAL(clientDisconnected()), this, SLOT(clientDisconnected()) );
	disconnect(m_listener, SIGNAL(messageRecieved(OIGTLMessage *)), this, SIGNAL(messageRecieved(OIGTLMessage *)) );

	if (m_sender != NULL)
	{
		delete m_sender;
		m_sender = NULL;
	}

	if (m_listener != NULL)
	{
		delete m_listener;
		m_listener = NULL;
	}

	if (m_mutex != NULL)
	{
		delete m_mutex;
		m_mutex = NULL;
	}
}

bool OIGTLSocketObject::listenOnPort(int port)
{
	if (m_listener != NULL)
	{
		bool ok = m_listener->initialize(port);

		if (ok)
		{
			m_port = port;
			m_listening = true;
			return true;
		}
		else return false;
	}
	else return false;
}

bool OIGTLSocketObject::connectToRemote(QUrl url)
{
	if (m_sender != NULL)
	{
		char * hostname = const_cast<char *>(url.host().toStdString().c_str());
		int port = url.port();
		
		bool ok = m_sender->initialize(hostname, port);
		
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