#include "OIGTLSenderThread.h"

OIGTLSenderThread::OIGTLSenderThread(QObject *parent)
: OIGTLThreadBase(parent)
{
	m_sendingOnSocket = false;
}

OIGTLSenderThread::~OIGTLSenderThread(void)
{
}

bool OIGTLSenderThread::initialize(igtl::Socket::Pointer socket)
{
	if (socket.IsNull())
	{
		QLOG_ERROR() << "Cannot create a sender socket, invalid external socket specified" << endl;
		return false;
	}

	if (m_initialized == true)
	{
		QLOG_ERROR() << "Sender already in use!" << endl;
		return false;
	}

	m_extSocket.operator =(socket);
	m_sendingOnSocket = true;

	if (!activate())
		return false;

	return true;
}


bool OIGTLSenderThread::initialize(char* hostname, int port)
{
	if (port <= 0 || hostname == NULL)
	{
		QLOG_ERROR() << "Cannot create a sender socket, invalid hostname or port specified" << endl;
		return false;
    }

	if (m_initialized == true)
	{
		QLOG_ERROR() << "Sender already in use!" << endl;
		return false;
	}

	igtl::ClientSocket::Pointer cs = igtl::ClientSocket::New(); 

	int r = cs->ConnectToServer(hostname, port);

	if (r != 0)
    {
		QLOG_ERROR() << "Cannot create a sender socket, could not connect to server: " <<hostname << endl;
		m_port = -1;
		return false;
    }

	m_port = port;
	
	m_extSocket.operator =((igtl::Socket::Pointer) cs);
	m_sendingOnSocket = false;

	if (!activate())
		return false;
	
	emit connectedToRemote();
	return true;
}

void OIGTLSenderThread::startThread(void)
{
	if (m_initialized == false || m_running == true)
		return;

	m_running = true;
	this->start();

	QLOG_INFO() <<"Started listening at: " <<m_port <<"\n";
}

void OIGTLSenderThread::stopThread(void)
{
	// Quitting thread	
	QLOG_INFO() << "Quitting sender thread \n";

	m_running = false;
	
	if (!m_sendingOnSocket && m_extSocket.IsNotNull())
	{
		m_mutex->lock();
		
		if (m_extSocket.IsNotNull())
			m_extSocket->CloseSocket();

		m_mutex->unlock();
	}

	m_sendingOnSocket = false;
	m_sendQue.clear();

	m_port = -1;
	m_initialized = false;

	exit(0);
}

bool OIGTLSenderThread::activate(void)
{
	if (m_mutex == NULL)
	{
		QLOG_INFO() <<"Cannot activate sender, mutex not set" <<endl;
		return false;
	}

	if (m_port <= 0)
	{
		QLOG_INFO() <<"Cannot activate sender, port is invalid" <<endl;
		return false;
	}
		
	if (m_extSocket.IsNull())
	{
		QLOG_INFO() <<"Cannot activate listener, socket is invalid" <<endl;
		return false;
	}

	m_initialized = true;
	
	QLOG_INFO() <<"Sender successfully activated" <<endl;

	return true;
}

void OIGTLSenderThread::run(void)
{
	while (m_running == true && !m_sendQue.isEmpty())
	{
		if (m_extSocket.IsNull())
		{
			QLOG_ERROR() <<"Cannot send message: Disconnected from remote host" <<"\n";
			emit disconnectedFromRemote();
			break;
		}

		OIGTLMessage * msg = m_sendQue.takeFirst();
		if (msg != NULL)
		{
			igtl::MessageBase::Pointer igtMsg;
			igtMsg.operator =(msg->getMessagePointer());

			if (igtMsg.IsNotNull())
			m_mutex->lock();
			m_extSocket->Send(igtMsg->GetPackPointer(), igtMsg->GetPackSize());
			m_mutex->unlock();

			//*******************************
			// CHANGE THIS

			//delete msg;
			//*******************************
		}

		igtl::Sleep(100); // wait 
	}
	
	if (m_sendQue.isEmpty())
		emit sendingFinished();

}

void OIGTLSenderThread::sendMsg(OIGTLMessage * msg)
{
	m_sendQue.append(msg);
	this->startThread();
}
