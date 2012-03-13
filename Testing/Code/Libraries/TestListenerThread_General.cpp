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
#include "TestListenerThread_General.h"

#include <cmath>

TestListenerThread_General::TestListenerThread_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestListenerThread_General::~TestListenerThread_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestListenerThread_General::setupTest()
{
  //Nothing to do right now
}


void TestListenerThread_General::performTest()
{

  //Instanciate Listener thread
  std::cout <<++m_testCounter <<". Creating a ListenerThread object..";
  OIGTLListenerThreadTester * listenerThread = NULL;
  listenerThread = new OIGTLListenerThreadTester();
  if (listenerThread != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener thread
  std::cout <<++m_testCounter <<". Creating a ListenerThread object w/ parent..";
  delete listenerThread;
  listenerThread = NULL;
  listenerThread = new OIGTLListenerThreadTester(this);
  if (listenerThread != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener thread
  std::cout <<++m_testCounter <<". Trying to initialize listener w/out mutex..";
  std::string hname;
  hname.append("localhost");
  bool ok = listenerThread->initialize(3200);
  if (!ok && !listenerThread->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener thread
  std::cout <<++m_testCounter <<". Trying to initialize listener w/ invalid portnum..";
  QMutex mutex;
  listenerThread->setMutex(&mutex);
  ok = listenerThread->initialize(-100);
  if (!ok && !listenerThread->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener thread - final
  std::cout <<++m_testCounter <<". Trying to initialize listener correctly..";
  delete listenerThread;
  listenerThread = NULL;
  listenerThread = new OIGTLListenerThreadTester(this);
  listenerThread->setMutex(&mutex);
  hname.clear();
  hname.append("localhost");
  ok = listenerThread->initialize(3200);
  if (ok && listenerThread->isInitialized())
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if getPort() works
  std::cout <<++m_testCounter <<". Testing port settings..";
  if (listenerThread->getPort() == 3200)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try to init with fake external socket pointer
  std::cout <<++m_testCounter <<". Trying to init with fake external socket pointer..";
  delete listenerThread;
  listenerThread = NULL;
  listenerThread = new OIGTLListenerThreadTester(this);
  listenerThread->setMutex(&mutex);
  igtl::Socket::Pointer socket = igtl::Socket::New();
  ok = listenerThread->initialize(socket, 3200);
  if (!ok && !listenerThread->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try if startthread works (should not)
  std::cout <<++m_testCounter <<". Trying to start thread..";
  listenerThread->startThread();
  if (!listenerThread->isActive())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test listen interval settings
  std::cout <<++m_testCounter <<". Testing listen interval settings..";
  listenerThread->setListenInterval(4);
  if (listenerThread->getListenInterval() == 4)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test socket timeout settings
  std::cout <<++m_testCounter <<". Testing socket-op timeout settings..";
  listenerThread->setSocketTimeOut(4);
  if (listenerThread->getSocketTimeOut() == 4)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  quitTest();
}

void TestListenerThread_General::quitTest()
{
  emit done();
  if (m_testCounter > m_successCounter)
    exit(-1);
  else exit(0);
}