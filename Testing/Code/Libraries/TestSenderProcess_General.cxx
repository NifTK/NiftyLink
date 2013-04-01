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
#include "TestSenderProcess_General.h"

#include <cmath>

TestSenderProcess_General::TestSenderProcess_General(void)
{
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestSenderProcess_General::~TestSenderProcess_General(void)
{
  std::cerr <<"Test class destructor";
}


void TestSenderProcess_General::SetupTest()
{
  //Nothing to do right now
}


void TestSenderProcess_General::PerformTest()
{

  //Instanciate sender Process
  std::cout <<++m_TestCounter <<". Creating a SenderProcess object..";
  NiftyLinkSenderProcessTester * senderProcess = NULL;
  senderProcess = new NiftyLinkSenderProcessTester();
  if (senderProcess != NULL)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process
  std::cout <<++m_TestCounter <<". Creating a SenderProcess object w/ parent..";
  delete senderProcess;
  senderProcess = NULL;
  senderProcess = new NiftyLinkSenderProcessTester(this);
  if (senderProcess != NULL)
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process
  std::cout <<++m_TestCounter <<". Trying to initialize sender w/out mutex..";
  std::string hname;
  hname.append("localhost");
  bool ok = senderProcess->Initialize(hname, 3200);
  if (!ok && !senderProcess->IsInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process
  std::cout <<++m_TestCounter <<". Trying to initialize sender w/ random hostname..";
  QMutex mutex;
  senderProcess->SetMutex(&mutex);
  hname.clear();
  hname.append("randomhostname");
  ok = senderProcess->Initialize(hname, 3200);
  if (!ok && !senderProcess->IsInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Instanciate sender Process - final
  std::cout <<++m_TestCounter <<". Trying to initialize sender correctly..";
  delete senderProcess;
  senderProcess = NULL;
  senderProcess = new NiftyLinkSenderProcessTester(this);
  senderProcess->SetMutex(&mutex);
  hname.clear();
  hname.append("localhost");
  ok = senderProcess->Initialize(hname, 3200);
  if (ok && senderProcess->IsInitialized())
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test if GetPort() works
  std::cout <<++m_TestCounter <<". Testing port settings..";
  if (senderProcess->GetPort() == 3200)
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try to init with fake external socket pointer
  std::cout <<++m_TestCounter <<". Trying to init with fake external socket pointer..";
  delete senderProcess;
  senderProcess = NULL;
  senderProcess = new NiftyLinkSenderProcessTester(this);
  senderProcess->SetMutex(&mutex);
  igtl::Socket::Pointer socket = igtl::Socket::New();
  ok = senderProcess->Initialize(socket, 3200);
  if (!ok && !senderProcess->IsInitialized())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Try if startProcess works (should not)
  std::cout <<++m_TestCounter <<". Trying to start Process..";
  senderProcess->StartProcess();
  if (!senderProcess->IsActive())
  { std::cout <<" NOT POSSIBLE: OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test connection timeout settings
  std::cout <<++m_TestCounter <<". Testing connection timeout settings..";
  senderProcess->SetConnectTimeOut(3);
  if (senderProcess->GetConnectTimeOut() == 3)
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  //Test socket timeout settings
  std::cout <<++m_TestCounter <<". Testing socket-op timeout settings..";
  senderProcess->SetSocketTimeOut(4);
  if (senderProcess->GetSocketTimeOut() == 4)
  { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  QuitTest();
}

void TestSenderProcess_General::QuitTest()
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
