/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef TestImagingSender_h
#define TestImagingSender_h

#include <QObject>
#include <QtGui/QImage>
#include <igtlTimeStamp.h>
#include "NiftyLinkSocketObject.h"

class TestImagingSender : public QObject
{

  Q_OBJECT

public:
  TestImagingSender(const QImage* image,
                    const QString& hostName,
                    const int& portNumber,
                    const int& numberOfIterations
                   );
  virtual ~TestImagingSender();

  inline void SetOutfilename(std::string outf) { m_OutFileName = outf; }

signals:
  void Done();

public slots:

  void Run();
  void OnMessageSent(unsigned long long timestamp);
  void OnConnectToRemote();
  void OnLostConnectionToRemote();
  void OnShutdownSender();

  void StoreMessageAccessTimes(QString);

private:

  bool Setup();
  void SendData(const int& numberOfIterations);
  void FinishUp();

  NiftyLinkSocketObject    m_Socket;
  const QImage*            m_Image;
  const QString            m_HostName;
  const int                m_PortNumber;
  const int                m_NumberOfIterations;
  int                      m_NumberSent;

  double                   m_MessageSize;
  QStringList              m_MsgAccessTimes;
  std::string              m_OutFileName;
  
  igtl::TimeStamp::Pointer m_TimeZero;
};

#endif // TestImagingSender_h
