#include "OIGTLSenderThread.h"

OIGTLSenderThread::OIGTLSenderThread(QObject *parent)
: OIGTLThreadBase(parent)
{
	m_sendingOnSocket = false;
}

OIGTLSenderThread::~OIGTLSenderThread(void)
{
	m_extSocket.operator =(NULL);
}

bool OIGTLSenderThread::initialize(igtl::Socket::Pointer socket, int port)
{
	if (socket.IsNull())
	{
		QLOG_ERROR() <<objectName() <<": " << "Cannot create a sender socket, invalid external socket specified" << endl;
		return false;
	}

	if (m_initialized == true)
	{
		QLOG_ERROR() <<objectName() <<": " << "Sender already in use!" << endl;
		return false;
	}

	m_extSocket.operator =(socket);
    m_extSocket->SetTimeout(10);
	m_sendingOnSocket = true;
        m_port = port;

	if (!activate())
		return false;

	return true;
}


bool OIGTLSenderThread::initialize(std::string &hostname, int port)
{
	if (port <= 0 || hostname.empty())
	{
		QLOG_ERROR() <<objectName() <<": " << "Cannot create a sender socket, invalid hostname or port specified" << endl;
		QLOG_ERROR() <<objectName() <<": " << "Hostname: " <<hostname.c_str() << endl;
		QLOG_ERROR() <<objectName() <<": " << "Port: " <<port << endl;
		return false;
    }

	//QLOG_ERROR() <<objectName() <<": " << "Hostname: " <<hostname << endl;
	//QLOG_ERROR() <<objectName() <<": " << "Port: " <<port << endl;

	if (m_initialized == true)
	{
		QLOG_ERROR() <<objectName() <<": " << "Sender already in use!" << endl;
		return false;
	}

	igtl::ClientSocket::Pointer cs = igtl::ClientSocket::New(); 
    cs->SetTimeout(10);
	int r = cs->ConnectToServer(hostname.c_str(), port);

	if (r != 0)
    {
		QLOG_ERROR() <<objectName() <<": " << "Cannot create a sender socket, could not connect to server: " <<hostname.c_str() << endl;
		m_port = -1;
		return false;
    }

	m_port = port;
	
	m_extSocket.operator =((igtl::Socket::Pointer) cs);
    m_extSocket->SetTimeout(10);
	m_sendingOnSocket = false;

	if (!activate())
		return false;
	
	emit connectedToRemote();
	return true;
}

void OIGTLSenderThread::startThread(void)
{
    if (m_initialized == false) // || m_running == true)
        return;

	m_running = true;
	this->start();

	if (!m_sendingOnSocket)
		QLOG_INFO() <<objectName() <<": " <<"Started sender at: " <<m_port <<"\n";
	else
		QLOG_INFO() <<objectName() <<": " <<"Started sender on socket" <<"\n";
}

void OIGTLSenderThread::stopThread(void)
{
	// Quitting thread	
	QLOG_INFO() <<objectName() <<": " << "Quitting sender thread \n";

	m_running = false;
	
	if (!m_sendingOnSocket && m_extSocket.IsNotNull())
	{
        int err = 0;
        QLOG_INFO() <<objectName() <<": " << "Closing socket... \n";

        m_mutex->lock();
        if (m_extSocket.IsNotNull())
		{
            err = m_extSocket->CloseSocket();
			m_extSocket.operator =(NULL);
		}
		m_mutex->unlock();

        if (err != 0)
            QLOG_ERROR() <<objectName() <<"CloseSocket returned with error: " <<err;
        else
            QLOG_INFO() <<objectName() <<"CloseSocket returned with error: " <<err;
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
		QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, mutex not set" <<endl;
		return false;
	}

	if (!m_sendingOnSocket && m_port <= 0)
	{
		QLOG_INFO() <<objectName() <<": " <<"Cannot activate sender, port is invalid" <<endl;
		return false;
	}
		
	if (m_extSocket.IsNull())
	{
		QLOG_INFO() <<objectName() <<": " <<"Cannot activate listener, socket is invalid" <<endl;
		return false;
	}

	m_initialized = true;
	
	QLOG_INFO() <<objectName() <<": " <<"Sender successfully activated" <<endl;

	return true;
}

void OIGTLSenderThread::run(void)
{
	while (m_running == true && !m_sendQue.isEmpty())
	{
		if (m_extSocket.IsNull())
		{
			QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: Disconnected from remote host" <<"\n";
			emit disconnectedFromRemote();
			break;
		}

        QLOG_INFO() <<objectName() <<": Messages in sendque: " << m_sendQue.count();

        m_queueMutex.lock();
        OIGTLMessage::Pointer msg;
        msg.operator =(m_sendQue.takeFirst());

        m_queueMutex.unlock();

		if (msg.operator!=(NULL))
		{
			igtl::MessageBase::Pointer igtMsg;
			msg->getMessagePointer(igtMsg);

			if (igtMsg.IsNotNull())
			{
				//m_mutex->tryLock();
				m_mutex->lock();
				m_extSocket->Send(igtMsg->GetPackPointer(), igtMsg->GetPackSize());
				m_mutex->unlock();

				QLOG_INFO() <<objectName() <<": " <<"Message Sent" <<endl;
			}
            else
                QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: igtMsg is NULL" <<"\n";

			//*******************************
			// CHANGE THIS

			//delete msg;
			//*******************************
		}
        else
            QLOG_ERROR() <<objectName() <<": " <<"Cannot send message: invalid message" <<"\n";

        //igtl::Sleep(50); // wait
	}
	
	if (m_sendQue.isEmpty())
		emit sendingFinished();

	m_running = false;
	exit();

}

void OIGTLSenderThread::sendMsg(OIGTLMessage::Pointer msg)
{
    QLOG_INFO() <<objectName() <<": " <<"Got new message to send, putting it to send queue.";

    if (msg.operator !=(NULL))
        QLOG_INFO() <<"MSG_ID: " <<msg->getId() <<endl;
    else
    {
        QLOG_ERROR() <<objectName() <<": " <<"Invalid message arrived to send" <<"\n";
        return;
    }

    m_queueMutex.lock();
	m_sendQue.append(msg);
    m_queueMutex.unlock();
	
    //igtl::Sleep(100);

	this->startThread();
}
