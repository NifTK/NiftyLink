/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "TestServerQt.h"
#include <NiftyLinkUtils.h>
#include <QsLog.h>
#include <QsLogDest.h>
#include <QCoreApplication>

namespace niftk
{

//-----------------------------------------------------------------------------
TestServerQt::TestServerQt(const int& portNumber,
                           const bool& isEchoing,
                           const bool &doStats,
                           const int threshold,
                           QObject *parent)
: m_PortNumber(portNumber)
, m_IsEchoing(isEchoing)
, m_DoStats(doStats)
, m_StatsThreshold(threshold)
, m_Server(new NiftyLinkTcpServer(parent))
{
  this->setObjectName("TestServerQt");

  qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
  qRegisterMetaType<NiftyLinkMessageContainer::Pointer>("NiftyLinkMessageContainer::Pointer");

  m_StatsTimer = new QTimer();
  m_StatsTimer->setInterval(10000);
}


//-----------------------------------------------------------------------------
TestServerQt::~TestServerQt()
{
  m_StatsTimer->disconnect();
  delete m_StatsTimer;
}


//-----------------------------------------------------------------------------
void TestServerQt::Start()
{
  QLOG_INFO() << QObject::tr("%1::Start() - starting up.").arg(objectName());

  connect(m_Server, SIGNAL(MessageReceived(int,NiftyLinkMessageContainer::Pointer)), this, SLOT(OnMessageReceived(int,NiftyLinkMessageContainer::Pointer)), Qt::DirectConnection);
  connect(m_Server, SIGNAL(SocketError(int,QAbstractSocket::SocketError, QString)), this, SLOT(OnSocketError(int,QAbstractSocket::SocketError, QString)));
  connect(m_Server, SIGNAL(ClientConnected(int)), this, SLOT(OnClientConnected(int)));
  connect(m_Server, SIGNAL(SentKeepAlive()), this, SLOT(OnKeepAliveSent()));
  connect(m_Server, SIGNAL(NoIncomingData()), this, SLOT(OnNoIncomingData()));

  if (!m_Server->listen(QHostAddress::Any, m_PortNumber))
  {
    QLOG_ERROR() << QObject::tr("%1::Start() - Unable to start server, error code %2, error string %3").arg(objectName()).arg(m_Server->serverError()).arg(m_Server->errorString());
  }

  if (m_DoStats)
  {
    if (m_StatsThreshold > 1)
    {
      m_Server->SetNumberMessageReceivedThreshold(m_StatsThreshold);
    }
    else
    {
      connect(m_StatsTimer, SIGNAL(timeout()), m_Server, SLOT(OutputStats()));
      m_StatsTimer->start();
    }
  }

  QLOG_INFO() << QObject::tr("%1::Start() - started.").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServerQt::Stop()
{
  QLOG_INFO() << QObject::tr("%1::Stop() - Shutting down server.").arg(objectName());

  m_Server->Shutdown();
  QTimer::singleShot(2000, this, SLOT(KillApp()));

  QLOG_INFO() << QObject::tr("%1::Stop() - Shut down server.").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServerQt::KillApp()
{
  QLOG_INFO() << QObject::tr("%1::KillApp() - Killing app.").arg(objectName());
  QCoreApplication::quit();
}


//-----------------------------------------------------------------------------
void TestServerQt::OnClientConnected(int portNumber)
{
  QLOG_INFO() << QObject::tr("%1::OnClientConnected(%2).").arg(objectName()).arg(portNumber);
}


//-----------------------------------------------------------------------------
void TestServerQt::OnSocketError(int portNumber, QAbstractSocket::SocketError errorCode, QString errorString)
{
  QLOG_ERROR() << QObject::tr("%1::OnSocketError(port=%2, code=%3, string=%4).").arg(objectName()).arg(portNumber).arg(errorCode).arg(errorString);
}


//-----------------------------------------------------------------------------
void TestServerQt::OnMessageReceived(int portNumber, NiftyLinkMessageContainer::Pointer message)
{
  QLOG_DEBUG() << QObject::tr("%1::OnMessageReceived(%2, %3), type=%4").arg(objectName()).arg(portNumber).arg(message->GetNiftyLinkMessageId()).arg(message->GetMessage()->GetDeviceType());

  if (m_IsEchoing)
  {
    message->GetMessage()->Pack();
    m_Server->Send(message);
  }

  QCoreApplication::processEvents();
}


//-----------------------------------------------------------------------------
void TestServerQt::OnKeepAliveSent()
{
  QLOG_INFO() << QObject::tr("%1::OnKeepAliveSent()").arg(objectName());
}


//-----------------------------------------------------------------------------
void TestServerQt::OnNoIncomingData()
{
  QLOG_INFO() << QObject::tr("%1::OnNoIncomingData()").arg(objectName());
}

} // end namespace niftk

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 6) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <port> <echo> <stats> <threshold> <totalSeconds>"    << std::endl;
    std::cerr << "    <port>         : Port #"            << std::endl;
    std::cerr << "    <echo>         : is echoing [0|1]"  << std::endl;
    std::cerr << "    <stats>        : do stats [0|1]"    << std::endl;
    std::cerr << "    <threshold>    : output stats every <threshold> number of messages [-1=off]" << std::endl;
    std::cerr << "    <totalSeconds> : total time in seconds to run server for, [-1=forever]" << std::endl;
    exit(0);
  }

  int    port       = atoi(argv[1]);
  int    isEcho     = atoi(argv[2]);
  int    doStats    = atoi(argv[3]);
  int    threshold  = atoi(argv[4]);
  int    totalSeconds = atoi(argv[5]);

  std::cout << "TestServerQt: port = " << port << "." << std::endl;
  std::cout << "TestServerQt: echo = " << isEcho << "." << std::endl;
  std::cout << "TestServerQt: stats = " << doStats << "." << std::endl;
  std::cout << "TestServerQt: threshold = " << threshold << "." << std::endl;
  std::cout << "TestServerQt: totalSeconds = " << totalSeconds << "." << std::endl;
  std::cout << "TestServerQt: Instantiating server." << std::endl;

  bool isEchoing = (isEcho == 1 ? true : false);
  bool doStatistics = (doStats == 1 ? true : false);

  QCoreApplication app(argc, argv);

  QsLogging::Logger& logger = QsLogging::Logger::instance();
  logger.setLoggingLevel(QsLogging::InfoLevel);
  QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  logger.addDestination(debugDestination.get());

  niftk::TestServerQt server(port, isEchoing, doStatistics, threshold);

  // Setup timers.
  QTimer::singleShot(220, &server, SLOT(Start()));
  if (totalSeconds > 0)
  {
    QTimer::singleShot(totalSeconds*1000, &server, SLOT(Stop()));
  }

  // Go!
  int ret = app.exec();

  return ret;
}
