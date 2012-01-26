#ifndef OIGTLSENDERTHREAD_H
#define OIGTLSENDERTHREAD_H

#include "OIGTLThreadBase.h"

class OIGTLSenderThread : public OIGTLThreadBase
{
  Q_OBJECT

    friend class OIGTLSocketObject;

signals:
  void connectedToRemote(void);
  void cannotConnectToRemote();
  void disconnectedFromRemote(bool onPort);
  void sendingFinished(void);

protected:
  OIGTLSenderThread(QObject *parent = 0);
  ~OIGTLSenderThread(void);

  virtual void run(void);
  virtual bool initialize(igtl::Socket::Pointer socket = 0, int port = -1);
  bool initialize(std::string &hostname, int port = 0);

  virtual void setConnectTimeOut(int sec);
  virtual  int getConnectTimeOut(void);

protected slots:
  virtual void sendMsg(OIGTLMessage::Pointer);

  virtual void startThread(void);
  virtual void stopThread(void);

private:
  virtual bool activate(void);

private:
  igtl::ClientSocket::Pointer m_clientSocket;
  std::string m_hostname;
  bool m_sendingOnSocket;
  int  m_connectTimeout;

  QList<OIGTLMessage::Pointer> m_sendQue;
  QMutex m_queueMutex;
};

#endif
