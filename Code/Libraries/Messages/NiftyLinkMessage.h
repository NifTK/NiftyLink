/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkMessage_h
#define NiftyLinkMessage_h

#include <NiftyLinkCommonWin32ExportHeader.h>

// Qt related issues
#include <QtCore>
#include <QString>
#include <QObject>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>

// OpenIGTLink message types
#include <igtlBindMessage.h>
#include <igtlColorTableMessage.h>
#include <igtlImageMessage.h>
#include <igtlImageMetaMessage.h>
#include <igtlLabelMetaMessage.h>
#include <igtlPointMessage.h>
#include <igtlPositionMessage.h>
#include <igtlQuaternionTrackingDataMessage.h>
#include <igtlSensorMessage.h>
#include <igtlStatusMessage.h>
#include <igtlStringMessage.h>
#include <igtlTrackingDataMessage.h>
#include <igtlTrajectoryMessage.h>
#include <igtlTransformMessage.h>

// OpenIGTLink utils
#include <igtlOSUtil.h>
#include <igtlTimeStamp.h>


/**
* \class NiftyLinkMessage
* \brief Base class for wrapping OpenIGTLink message types, but it can be instanciated on its own too.
*
* NiftyLinkMessage is the base class for all NiftyLink message types. The aim is to wrap OpenIGTLink
* functionality into lightweight and easy to use QT-based message format. The NiftyLinkMessage is
* based on QSharedData, used along with QExplicitlySharedDataPointer which is basically the Qt
* version of the smartpointers. NiftyLinkMessage has all the required functions and data members to
* easily create, destroy, initialize, identify and tag message data.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessage : public QSharedData
{

public:

  // Typedefs that are common in all message types
  typedef NiftyLinkMessage Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which generates a timestamp and derives the message ID from it.
  NiftyLinkMessage(void);

  /// \brief Basic destructor.
  virtual ~NiftyLinkMessage(void);

  /// \brief Basic copy constructor required for this data type
  NiftyLinkMessage(const NiftyLinkMessage &other);

  /// \brief This function embeds an OpenIGTLink message pointer. mp needs to be packed!
  void SetMessagePointer(igtl::MessageBase::Pointer mp);

  /// \brief This function returns the embedded OpenIGTLink message pointer
  void GetMessagePointer(igtl::MessageBase::Pointer &mp);

  /// \brief This functions sets the sender's host name
  void ChangeHostName(QString hname);

  /// \brief This functions returns the sender's host name
  QString GetHostName(void);

  /// \brief Changes the message tag stored within the message (USE WITH CARE)
  void ChangeMessageType(QString type);

  /// \brief This function returns the currently set message tag
  QString GetMessageType(void);

  /// \brief This function is the set the port number on which the message was received
  void SetPort(int port);

  /// \brief This function returns the port number
  int GetPort(void);

  /// \brief This function is to mark when the first byte of the message has arrived
  void SetTimeArrived(igtl::TimeStamp::Pointer ts);

  /// \brief Returns the time when the message has arrived
  igtl::TimeStamp::Pointer GetTimeArrived(void);

  /// \brief This function is for setting the timestamp when the message was fully received
  void SetTimeReceived(igtl::TimeStamp::Pointer ts);

  /// \brief Returns the time when the message was received
  igtl::TimeStamp::Pointer GetTimeReceived(void);

  /// \brief Returns the time when the message was created
  igtl::TimeStamp::Pointer GetTimeCreated(void);

  /// \brief Returns the message ID
  igtlUint64 GetId(void);

  //STT related functions
  /// \brief Sets the frequency of the stream (used in STT_ messages)
  void SetResolution(igtlUint64 res);

  /// \brief Returns the frequency of the stream (used in STT_ messages)
  void GetResolution(igtlUint64 &res);

  /// \brief Function base for initilizing the message with test data (defined in NiftyLink Utils)
  virtual void InitializeWithTestData(void) {}

  /// \brief Function base for initilizing the message with random data (defined in NiftyLink Utils)
  virtual void InitializeWithRandomData(void) {}

  /// \brief Function to Update the timestamp and host name after changes
  void Update(QString hostname, igtl::TimeStamp::Pointer ts);

  /// \brief Function to Update the host name after changes
  void Update(QString hostname);

  /// \brief Function set message processed and to check status
  inline void SetProcessed(bool p)
  {
    m_Processed = p;
  }

  /// \brief Function to check if a message was processed
  bool GetProcessed()
  {
    return m_Processed;
  }

  /// \brief Set the name of the owner class as the message is passed back and forth
  void SetOwnerName(QString str);

  /// \brief Method to check who owns the message at the moment
  QString GetOwnerName(void)
  {
    return m_OwnerName;
  }

  /// \brief This method is to support debugging lags in the message transmission
  inline void TouchMessage(std::string who, igtl::TimeStamp::Pointer when) { m_AccessTimes[who] = when; }

  /// \brief This method returns the times in nanosecs when the message was touched
  /// The format is: ID, TimeCreated, EndPack, SendStart, SendFinish
  QString GetAccessTimes();

  /// \brief This method returns all timestamp details about the message
  QString GetDetailedAccessInfo();

  /// \brief Pack or unpack the message from its wire-format.
  void Pack();
  void Unpack();
  void SetPacked(bool packed);

protected:

  QString                    m_MessageType;
  igtl::MessageBase::Pointer m_Message;
  // To mark when the first byte of the message arrived to the socket
  igtl::TimeStamp::Pointer   m_TimeArrived;
  // To mark when the message was fully received
  igtl::TimeStamp::Pointer   m_TimeReceived;
  // To mark when the message was created
  igtl::TimeStamp::Pointer   m_TimeCreated;

  QString                    m_SenderHostName;
  int                        m_SenderPort;

  igtlUint64                 m_Id;
  igtlUint64                 m_Resolution;

  bool                       m_Processed;
  QString                    m_OwnerName;

  // try to keep track of whether a message has been packed for transmission, or unpacked on receive.
  // this should avoid having to repack messages for each getter/setter, just to keep a consistent state.
  // FIXME: this should really go to openigtlink somewhere!
  //        SetMessagePointer() may screw us over quite a bit
  bool                       m_IsPacked;

  std::map<std::string, igtl::TimeStamp::Pointer> m_AccessTimes;
};

Q_DECLARE_METATYPE(NiftyLinkMessage::Pointer);

#endif // NiftyLinkMessage_h
