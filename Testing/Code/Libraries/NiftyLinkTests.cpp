/*=============================================================================

 NiftyGuide: A software package for light-weight client applications for
             Image Guided Interventions, developed at University College London.

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

#include <QObject>
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QSignalSpy>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "igtlTransformMessage.h"
#include "igtlMath.h"

#include "OIGTLSocketObject.h"

#include "QsLog.h"
#include "QsLogDest.h"


int main(int argc, char **argv)
{
  QCoreApplication app(argc,argv);
  qRegisterMetaType<OIGTLMessage *>();
  //// init the logging mechanism
  //QsLogging::Logger& logger = QsLogging::Logger::instance();
  //logger.setLoggingLevel(QsLogging::TraceLevel);
  //const QString sLogPath(QDir(app.applicationDirPath()).filePath("niftylink_log.txt"));
  //QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
  //QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination() );
  //logger.addDestination(debugDestination.get());
  //logger.addDestination(fileDestination.get());

  //QLOG_INFO() << "Program started";

  OIGTLSocketObject * socket1 = new OIGTLSocketObject(&app);
  delete socket1;

  socket1 = new OIGTLSocketObject(&app);

  OIGTLSocketObject * socket2 = new OIGTLSocketObject(&app);
  MessageCatcher * catcher = new MessageCatcher();

  QObject::connect(socket1, SIGNAL(messageReceived(OIGTLMessage * )), catcher, SLOT(catchMessage(OIGTLMessage * )) );
  QSignalSpy * spy = new QSignalSpy(socket1, SIGNAL(messageReceived(OIGTLMessage * )));


  QUrl url;
  url.setHost(QString("localhost"));
  url.setPort(3200);
  
  socket1->listenOnPort(3200);
  socket2->connectToRemote(url);

  OIGTLMessage * msgToSend = new OIGTLMessage();
  msgToSend->setHostName(QString("MURBELLA_O"));
  msgToSend->setMessageType(QString("TRANSFORM"));

  igtl::TransformMessage::Pointer transMsg;
  transMsg = igtl::TransformMessage::New();
  transMsg->SetDeviceName("Tracker"); 

  igtl::Matrix4x4 matrix;
  GetRandomTestMatrix(matrix);
  transMsg->SetMatrix(matrix);
  transMsg->Pack();

  msgToSend->setMessagePointer((igtl::MessageBase::Pointer) transMsg);

  socket2->sendMessage(msgToSend);

  while (!catcher->isMessageValid())
  {
	igtl::Sleep(500);
	qDebug() <<"MessageSignal received: " <<spy->count() <<endl;
	qDebug() <<"TestSignalCalls from socketobject: " <<socket1->getTestSignalCalls() <<endl;
  }

  OIGTLMessage * recievedMsg = catcher->getMessage();

  //igtl::MessageBase::Pointer recievedIGTMsg = recievedMsg->getMessagePointer();

  //igtl::TransformMessage::Pointer receivedTransMsg;
  
  //receivedTransMsg.operator =(recievedIGTMsg.GetPointer());
  //
  //igtl::Matrix4x4 receivedMatrix;

  //receivedTransMsg->GetMatrix(receivedMatrix);

  //if (matrix != receivedMatrix)
  //{
	 // qDebug() <<"shit happenes";
  //}


  int ret = app.exec();

  return ret;
}
