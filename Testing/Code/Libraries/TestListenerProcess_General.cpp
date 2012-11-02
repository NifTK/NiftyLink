/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "stdlib.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include "TestListenerProcess_General.h"

#include <cmath>

TestListenerProcess_General::TestListenerProcess_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestListenerProcess_General::~TestListenerProcess_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestListenerProcess_General::setupTest()
{
  //Nothing to do right now
}


void TestListenerProcess_General::performTest()
{

  //Instanciate Listener Process
  std::cout <<++m_testCounter <<". Creating a ListenerProcess object..";
  OIGTLListenerProcessTester * listenerProcess = NULL;
  listenerProcess = new OIGTLListenerProcessTester();
  if (listenerProcess != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process
  std::cout <<++m_testCounter <<". Creating a ListenerProcess object w/ parent..";
  delete listenerProcess;
  listenerProcess = NULL;
  listenerProcess = new OIGTLListenerProcessTester(this);
  if (listenerProcess != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process
  std::cout <<++m_testCounter <<". Trying to initialize listener w/out mutex..";
  std::string hname;
  hname.append("localhost");
  bool ok = listenerProcess->initialize(3200);
  if (!ok && !listenerProcess->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process
  std::cout <<++m_testCounter <<". Trying to initialize listener w/ invalid portnum..";
  QMutex mutex;
  listenerProcess->setMutex(&mutex);
  ok = listenerProcess->initialize(-100);
  if (!ok && !listenerProcess->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process - final
  std::cout <<++m_testCounter <<". Trying to initialize listener correctly..";
  delete listenerProcess;
  listenerProcess = NULL;
  listenerProcess = new OIGTLListenerProcessTester(this);
  listenerProcess->setMutex(&mutex);
  hname.clear();
  hname.append("localhost");
  ok = listenerProcess->initialize(3200);
  if (ok && listenerProcess->isInitialized())
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if getPort() works
  std::cout <<++m_testCounter <<". Testing port settings..";
  if (listenerProcess->getPort() == 3200)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try to init with fake external socket pointer
  std::cout <<++m_testCounter <<". Trying to init with fake external socket pointer..";
  delete listenerProcess;
  listenerProcess = NULL;
  listenerProcess = new OIGTLListenerProcessTester(this);
  listenerProcess->setMutex(&mutex);
  igtl::Socket::Pointer socket = igtl::Socket::New();
  ok = listenerProcess->initialize(socket, 3200);
  if (!ok && !listenerProcess->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try if startProcess works (should not)
  std::cout <<++m_testCounter <<". Trying to start Process..";
  listenerProcess->startProcess();
  if (!listenerProcess->isActive())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test listen interval settings
  std::cout <<++m_testCounter <<". Testing listen interval settings..";
  listenerProcess->setListenInterval(4);
  if (listenerProcess->getListenInterval() == 4)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test socket timeout settings
  std::cout <<++m_testCounter <<". Testing socket-op timeout settings..";
  listenerProcess->setSocketTimeOut(4);
  if (listenerProcess->getSocketTimeOut() == 4)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  quitTest();
}

void TestListenerProcess_General::quitTest()
{
  emit done();

  if (m_testCounter > m_successCounter)
  {
    std::cout <<"\n\n\n";
    std::cout <<"****************************************************\n";
    std::cout <<"**************** TESTING FINISHED: *****************\n";
    std::cout <<"***************** " <<(m_testCounter - m_successCounter) << " TEST(S) FAILED *****************\n";
    std::cout <<"****************************************************\n";
    exit(-1);
  }
  else
  {
    std::cout <<"\n\n\n";
    std::cout <<"****************************************************\n";
    std::cout <<"**************** TESTING FINISHED: *****************\n";
    std::cout <<"********* ALL TESTS COMPLETED SUCCESSFULLY *********\n";
    std::cout <<"****************************************************\n";
    exit(0);
  }
}
