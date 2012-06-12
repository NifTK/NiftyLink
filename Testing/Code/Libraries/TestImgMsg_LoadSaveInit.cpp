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
#include "TestImgMsg_LoadSaveInit.h"

#include <cmath>

TestImgMsg_LoadSaveInit::TestImgMsg_LoadSaveInit(void)
{
  m_testCounter = 0;
  m_successCounter = 0;
}

TestImgMsg_LoadSaveInit::~TestImgMsg_LoadSaveInit(void)
{
  std::cerr <<"Test class destructor";
}


void TestImgMsg_LoadSaveInit::setupTest()
{
  //Nothing to do right now
}


void TestImgMsg_LoadSaveInit::performTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_testCounter <<". Creating image message..";
  OIGTLImageMessage::Pointer imageMsg;
  imageMsg.reset();
  imageMsg = (OIGTLImageMessage::Pointer(new OIGTLImageMessage()));
  
  if (imageMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  igtl::MessageBase::Pointer p;
  QImage img1, img2;
  img1.load(":/NiftyLink/UCL_LOGO.tif");
  
  std::cout <<++m_testCounter <<". Testing set / get QImage..";
  imageMsg->setQImage(img1);
  imageMsg->getMessagePointer(p);
  img2 = imageMsg->getQImage();
  if (p.IsNotNull() && !img2.isNull()  && img1.operator ==(img2))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting test image data..";
  imageMsg->initializeWithTestData();
  imageMsg->getMessagePointer(p);
  img2 = imageMsg->getQImage();
  if (p.IsNotNull() && !img2.isNull()  && img1.operator ==(img2))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Setting random image data..";
  imageMsg->initializeWithRandomData();
  imageMsg->save(QString("some_random_filename.png"));
  img2 = imageMsg->getQImage();
  if (p.IsNotNull() && !img2.isNull() && img1.operator !=(img2))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Saving and loading test image data..";
  imageMsg->initializeWithTestData();
  imageMsg->save(QString("some_random_filename.png"));
  imageMsg->load(QString("some_random_filename.png"));
  img2 = imageMsg->getQImage();
  if (p.IsNotNull() && !img2.isNull() && img1.operator ==(img2))
    { std::cout <<" OK\n"; m_successCounter++; }
  else
     std::cout <<" FAILED\n";

  //***********************************************
  std::cout <<++m_testCounter <<". Deleting messages..";
  imageMsg.reset();
  imageMsg.operator =(NULL);
  
  if (imageMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_successCounter++; }

  //***********************************************

  quitTest();
}

void TestImgMsg_LoadSaveInit::quitTest()
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
