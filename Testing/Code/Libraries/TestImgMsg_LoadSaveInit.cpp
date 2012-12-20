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
#include <QImage>
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


void TestImgMsg_LoadSaveInit::setupTest(int argc, char **argv)
{
  m_argc = argc;
  m_argv = argv;
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
  std::cerr <<++m_testCounter <<". Testing specifically with supplied image file 1..";
  QString imageFileName = m_argv[1];
  QImage inputImage(imageFileName);

  // NOTE: At the moment, we have to convert image to ARGB32.
  QImage convertedInputImage = inputImage.convertToFormat(QImage::Format_ARGB32);

  imageMsg->setQImage(convertedInputImage);
  QImage outputImage = imageMsg->getQImage();

  if (convertedInputImage == outputImage)
  {
    std::cerr <<" OK\n"; m_successCounter++;
  }
  else
  {
    std::cerr <<" FAILED\n";
  }

  //***********************************************
  std::cerr <<++m_testCounter <<". Testing specifically with supplied image file 1 " << m_argv[1] << ", ... and doing Format_Indexed8 .." << std::endl;
  QString imageFileName2 = m_argv[1];
  QImage inputImage2(imageFileName2);

  std::cerr << "... Image format=" << inputImage2.format() << std::endl;

  convertedInputImage = inputImage2.convertToFormat(QImage::Format_Indexed8);

  imageMsg->setQImage(convertedInputImage);
  outputImage = imageMsg->getQImage();

  if (convertedInputImage == outputImage)
  {
    std::cerr <<" OK\n"; m_successCounter++;
  }
  else
  {
    std::cerr <<" FAILED\n";
  }

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
