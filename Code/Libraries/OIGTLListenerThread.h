#ifndef OIGTLLISTENERTHREAD_H
#define OIGTLLISTENERTHREAD_H

#include "OIGTLThreadBase.h"
#include "igtlBindMessage.h"
#include "igtlPointMessage.h"
#include "igtlStringMessage.h"

class OIGTLListenerThread : public OIGTLThreadBase
{
  Q_OBJECT

    friend class OIGTLSocketObject;

signals:
  void clientConnected(void);
  void clientDisconnected(void);
  void messageReceived(OIGTLMessage::Pointer msg);

  void testSignal(void);

protected:
  OIGTLListenerThread(QObject *parent = 0);
  ~OIGTLListenerThread(void);

  virtual void run(void);
  virtual bool initialize(igtl::Socket::Pointer socket = 0, int port = -1);

  virtual void setListenInterval(int msec);
  virtual  int getListenInterval(void);

  bool initialize(int port);

protected slots:
  virtual void startThread(void);
  virtual void stopThread(void);

protected:

private:
  virtual bool activate(void);

  void listenOnPort();
  void listenOnSocket();
  void receiveMessage();

private:
  igtl::ServerSocket::Pointer  m_serverSocket;
  bool m_listeningOnPort;
  int m_listenInterval;
};

#endif
