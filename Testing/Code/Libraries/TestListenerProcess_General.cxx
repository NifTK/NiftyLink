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
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestListenerProcess_General::~TestListenerProcess_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestListenerProcess_General::SetupTest()
{
  //Nothing to do right now
}


void TestListenerProcess_General::PerformTest()
{

  //Instanciate Listener Process
  std::cout <<++m_TestCounter <<". Creating a ListenerProcess object..";
  NiftyLinkListenerProcessTester * listenerProcess = NULL;
  listenerProcess = new NiftyLinkListenerProcessTester();
  if (listenerProcess != NULL)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process
  std::cout <<++m_TestCounter <<". Creating a ListenerProcess object w/ parent..";
  delete listenerProcess;
  listenerProcess = NULL;
  listenerProcess = new NiftyLinkListenerProcessTester(this);
  if (listenerProcess != NULL)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process
  std::cout <<++m_TestCounter <<". Trying to initialize listener w/out mutex..";
  std::string hname;
  hname.append("localhost");
  bool ok = listenerProcess->Initialize(3200);
  if (!ok && !listenerProcess->IsInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process
  std::cout <<++m_TestCounter <<". Trying to initialize listener w/ invalid portnum..";
  QMutex mutex;
  listenerProcess->SetMutex(&mutex);
  ok = listenerProcess->Initialize(-100);
  if (!ok && !listenerProcess->IsInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate listener Process - final
  std::cout <<++m_TestCounter <<". Trying to initialize listener correctly..";
  delete listenerProcess;
  listenerProcess = NULL;
  listenerProcess = new NiftyLinkListenerProcessTester(this);
  listenerProcess->SetMutex(&mutex);
  hname.clear();
  hname.append("localhost");
  ok = listenerProcess->Initialize(3200);
  if (ok && listenerProcess->IsInitialized())
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if GetPort() works
  std::cout <<++m_TestCounter <<". Testing port settings..";
  if (listenerProcess->GetPort() == 3200)
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try to init with fake external socket pointer
  std::cout <<++m_TestCounter <<". Trying to init with fake external socket pointer..";
  delete listenerProcess;
  listenerProcess = NULL;
  listenerProcess = new NiftyLinkListenerProcessTester(this);
  listenerProcess->SetMutex(&mutex);
  igtl::Socket::Pointer socket = igtl::Socket::New();
  ok = listenerProcess->Initialize(socket, 3200);
  if (!ok && !listenerProcess->IsInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try if startProcess works (should not)
  std::cout <<++m_TestCounter <<". Trying to start Process..";
  listenerProcess->StartProcess();
  if (!listenerProcess->IsActive())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test listen interval settings
  std::cout <<++m_TestCounter <<". Testing listen interval settings..";
  listenerProcess->SetListenInterval(4);
  if (listenerProcess->GetListenInterval() == 4)
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test socket timeout settings
  std::cout <<++m_TestCounter <<". Testing socket-op timeout settings..";
  listenerProcess->SetSocketTimeOut(4);
  if (listenerProcess->GetSocketTimeOut() == 4)
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  QuitTest();
}

void TestListenerProcess_General::QuitTest()
{
  emit Done();

  if (m_TestCounter > m_SuccessCounter)
  {
    std::cout <<"\n\n\n";
    std::cout <<"****************************************************\n";
    std::cout <<"**************** TESTING FINISHED: *****************\n";
    std::cout <<"***************** " <<(m_TestCounter - m_SuccessCounter) << " TEST(S) FAILED *****************\n";
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
