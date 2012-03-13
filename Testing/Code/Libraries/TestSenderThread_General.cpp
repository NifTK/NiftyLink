/*=============================================================================

NiftyLink:  A software library to facilitate communication over OpenIGTLink.

http://cmic.cs.ucl.ac.uk/
http://www.ucl.ac.uk/

Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
Revision          : $Revision: 3300 $
Last modified by  : $Author: mjc $

Original author   : m.clarkson@ucl.ac.uk

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

============================================================================*/

#include "stdlib.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include "TestSenderThread_General.h"

#include <cmath>

TestSenderThread_General::TestSenderThread_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestSenderThread_General::~TestSenderThread_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestSenderThread_General::setupTest()
{
  //Nothing to do right now
}


void TestSenderThread_General::performTest()
{

  //Instanciate sender thread
  std::cout <<++m_testCounter <<". Creating a SenderThread object..";
  OIGTLSenderThreadTester * senderThread = NULL;
  senderThread = new OIGTLSenderThreadTester();
  if (senderThread != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender thread
  std::cout <<++m_testCounter <<". Creating a SenderThread object w/ parent..";
  delete senderThread;
  senderThread = NULL;
  senderThread = new OIGTLSenderThreadTester(this);
  if (senderThread != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender thread
  std::cout <<++m_testCounter <<". Trying to initialize sender w/out mutex..";
  std::string hname;
  hname.append("localhost");
  bool ok = senderThread->initialize(hname, 3200);
  if (!ok && !senderThread->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender thread
  std::cout <<++m_testCounter <<". Trying to initialize sender w/ random hostname..";
  QMutex mutex;
  senderThread->setMutex(&mutex);
  hname.clear();
  hname.append("randomhostname");
  ok = senderThread->initialize(hname, 3200);
  if (!ok && !senderThread->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender thread - final
  std::cout <<++m_testCounter <<". Trying to initialize sender correctly..";
  delete senderThread;
  senderThread = NULL;
  senderThread = new OIGTLSenderThreadTester(this);
  senderThread->setMutex(&mutex);
  hname.clear();
  hname.append("localhost");
  ok = senderThread->initialize(hname, 3200);
  if (ok && senderThread->isInitialized())
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if getPort() works
  std::cout <<++m_testCounter <<". Testing port settings..";
  if (senderThread->getPort() == 3200)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try to init with fake external socket pointer
  std::cout <<++m_testCounter <<". Trying to init with fake external socket pointer..";
  delete senderThread;
  senderThread = NULL;
  senderThread = new OIGTLSenderThreadTester(this);
  senderThread->setMutex(&mutex);
  igtl::Socket::Pointer socket = igtl::Socket::New();
  ok = senderThread->initialize(socket, 3200);
  if (!ok && !senderThread->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try if startthread works (should not)
  std::cout <<++m_testCounter <<". Trying to start thread..";
  senderThread->startThread();
  if (!senderThread->isActive())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test connection timeout settings
  std::cout <<++m_testCounter <<". Testing connection timeout settings..";
  senderThread->setConnectTimeOut(3);
  if (senderThread->getConnectTimeOut() == 3)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test socket timeout settings
  std::cout <<++m_testCounter <<". Testing socket-op timeout settings..";
  senderThread->setSocketTimeOut(4);
  if (senderThread->getSocketTimeOut() == 4)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  quitTest();
}

void TestSenderThread_General::quitTest()
{
  emit done();
  if (m_testCounter > m_successCounter)
    exit(-1);
  else exit(0);
}