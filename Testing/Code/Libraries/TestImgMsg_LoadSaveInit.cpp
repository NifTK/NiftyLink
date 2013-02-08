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
  m_TestCounter = 0;
  m_SuccessCounter = 0;
}

TestImgMsg_LoadSaveInit::~TestImgMsg_LoadSaveInit(void)
{
  std::cerr <<"Test class destructor";
}


void TestImgMsg_LoadSaveInit::SetupTest(int argc, char **argv)
{
  m_Argc = argc;
  m_Argv = argv;
}


void TestImgMsg_LoadSaveInit::PerformTest()
{
  //Create image message and initialize with test data
  std::cout <<++m_TestCounter <<". Creating image message..";
  NiftyLinkImageMessage::Pointer imageMsg;
  imageMsg.reset();
  imageMsg = (NiftyLinkImageMessage::Pointer(new NiftyLinkImageMessage()));
  
  if (imageMsg.operator !=(NULL))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
    std::cout <<" FAILED\n";

  //***********************************************
  igtl::MessageBase::Pointer p;
  QImage img1, img2;
  img1.load(":/NiftyLink/UCL_LOGO.tif");
  
  std::cout <<++m_TestCounter <<". Testing set / get QImage..";
  imageMsg->SetQImage(img1);
  imageMsg->GetMessagePointer(p);
  img2 = imageMsg->GetQImage();
  if (p.IsNotNull() && !img2.isNull()  && img1.operator ==(img2))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting test image data..";
  imageMsg->InitializeWithTestData();
  imageMsg->GetMessagePointer(p);
  img2 = imageMsg->GetQImage();
  if (p.IsNotNull() && !img2.isNull()  && img1.operator ==(img2))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  //***********************************************
  std::cout <<++m_TestCounter <<". Setting random image data..";
  imageMsg->InitializeWithRandomData();
  imageMsg->Save(QString("some_random_filename.png"));
  img2 = imageMsg->GetQImage();
  if (p.IsNotNull() && !img2.isNull() && img1.operator !=(img2))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  //***********************************************
  std::cout <<++m_TestCounter <<". Saving and loading test image data..";
  imageMsg->InitializeWithTestData();
  imageMsg->Save(QString("some_random_filename.png"));
  imageMsg->Load(QString("some_random_filename.png"));
  img2 = imageMsg->GetQImage();
  if (p.IsNotNull() && !img2.isNull() && img1.operator ==(img2))
    { std::cout <<" OK\n"; m_SuccessCounter++; }
  else
     std::cout <<" FAILED\n";
  //***********************************************
  std::cerr <<++m_TestCounter <<". Testing specifically with supplied image file 1..";
  QString imageFileName = m_Argv[1];
  QImage inputImage(imageFileName);

  // NOTE: At the moment, we have to convert image to ARGB32.
  QImage convertedInputImage = inputImage.convertToFormat(QImage::Format_ARGB32);

  imageMsg->SetQImage(convertedInputImage);
  QImage outputImage = imageMsg->GetQImage();

  if (convertedInputImage == outputImage)
  {
    std::cerr <<" OK\n"; m_SuccessCounter++;
  }
  else
  {
    std::cerr <<" FAILED\n";
  }

  //***********************************************
  std::cerr <<++m_TestCounter <<". Testing specifically with supplied image file 1 " << m_Argv[1] << ", ... and doing Format_Indexed8 .." << std::endl;
  QString imageFileName2 = m_Argv[1];
  QImage inputImage2(imageFileName2);

  std::cerr << "... Image format=" << inputImage2.format() << std::endl;

  convertedInputImage = inputImage2.convertToFormat(QImage::Format_Indexed8);

  imageMsg->SetQImage(convertedInputImage);
  outputImage = imageMsg->GetQImage();

  if (convertedInputImage == outputImage)
  {
    std::cerr <<" OK\n"; m_SuccessCounter++;
  }
  else
  {
    std::cerr <<" FAILED\n";
  }

  //***********************************************
  std::cout <<++m_TestCounter <<". Deleting messages..";
  imageMsg.reset();
  imageMsg.operator =(NULL);

  if (imageMsg.operator !=(NULL))
    std::cout <<" FAILED\n";
  else
    { std::cout <<" OK\n"; m_SuccessCounter++; }

  //***********************************************

  QuitTest();
}

void TestImgMsg_LoadSaveInit::QuitTest()
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
