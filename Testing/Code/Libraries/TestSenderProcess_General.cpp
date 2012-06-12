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
#include "TestSenderProcess_General.h"

#include <cmath>

TestSenderProcess_General::TestSenderProcess_General(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestSenderProcess_General::~TestSenderProcess_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestSenderProcess_General::setupTest()
{
  //Nothing to do right now
}


void TestSenderProcess_General::performTest()
{

  //Instanciate sender Process
  std::cout <<++m_testCounter <<". Creating a SenderProcess object..";
  OIGTLSenderProcessTester * senderProcess = NULL;
  senderProcess = new OIGTLSenderProcessTester();
  if (senderProcess != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process
  std::cout <<++m_testCounter <<". Creating a SenderProcess object w/ parent..";
  delete senderProcess;
  senderProcess = NULL;
  senderProcess = new OIGTLSenderProcessTester(this);
  if (senderProcess != NULL)
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process
  std::cout <<++m_testCounter <<". Trying to initialize sender w/out mutex..";
  std::string hname;
  hname.append("localhost");
  bool ok = senderProcess->initialize(hname, 3200);
  if (!ok && !senderProcess->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process
  std::cout <<++m_testCounter <<". Trying to initialize sender w/ random hostname..";
  QMutex mutex;
  senderProcess->setMutex(&mutex);
  hname.clear();
  hname.append("randomhostname");
  ok = senderProcess->initialize(hname, 3200);
  if (!ok && !senderProcess->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process - final
  std::cout <<++m_testCounter <<". Trying to initialize sender correctly..";
  delete senderProcess;
  senderProcess = NULL;
  senderProcess = new OIGTLSenderProcessTester(this);
  senderProcess->setMutex(&mutex);
  hname.clear();
  hname.append("localhost");
  ok = senderProcess->initialize(hname, 3200);
  if (ok && senderProcess->isInitialized())
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if getPort() works
  std::cout <<++m_testCounter <<". Testing port settings..";
  if (senderProcess->getPort() == 3200)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try to init with fake external socket pointer
  std::cout <<++m_testCounter <<". Trying to init with fake external socket pointer..";
  delete senderProcess;
  senderProcess = NULL;
  senderProcess = new OIGTLSenderProcessTester(this);
  senderProcess->setMutex(&mutex);
  igtl::Socket::Pointer socket = igtl::Socket::New();
  ok = senderProcess->initialize(socket, 3200);
  if (!ok && !senderProcess->isInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try if startProcess works (should not)
  std::cout <<++m_testCounter <<". Trying to start Process..";
  senderProcess->startProcess();
  if (!senderProcess->isActive())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test connection timeout settings
  std::cout <<++m_testCounter <<". Testing connection timeout settings..";
  senderProcess->setConnectTimeOut(3);
  if (senderProcess->getConnectTimeOut() == 3)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test socket timeout settings
  std::cout <<++m_testCounter <<". Testing socket-op timeout settings..";
  senderProcess->setSocketTimeOut(4);
  if (senderProcess->getSocketTimeOut() == 4)
  { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  quitTest();
}

void TestSenderProcess_General::quitTest()
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
