/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkMessageContainer_h
#define NiftyLinkMessageContainer_h

#include <NiftyLinkCommonWin32ExportHeader.h>

#include <igtlObject.h>
#include <igtlOSUtil.h>
#include <igtlTimeStamp.h>
#include <igtlMessageBase.h>

#include <QtCore>
#include <QString>
#include <QObject>
#include <QStringList>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>

#include <map>

namespace niftk
{
/**
* \class NiftyLinkMessageContainer
* \brief Wrapper for OpenIGTLink message types.
*
* The aim of this class is to provide any additional information
* that we require on an image, that is not present in OpenIGTLink.
* It should wrap ANY OpenIGTLink message type, and should not refer
* to any specific OpenIGTLink message type, so that if OpenIGTLink
* introduces more message types derived from igtl::MessageBase,
* then this class does not need modifying.
*
* This class should NOT be used across a Qt::QueuedConnection,
* as even though there are default constructor, copy and
* assignment operators, it does NOT survive the Qt Meta-Object system.
*
* We currently store a smart pointer to the OpenIGTLink image, so
* copy operators are shallow, copying the value of this pointer.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkMessageContainer : public QSharedData
{

public:

  // Typedefs that are common in all message types
  typedef NiftyLinkMessageContainer Self;
  typedef QExplicitlySharedDataPointer<Self> Pointer;
  typedef QExplicitlySharedDataPointer<const Self> ConstPointer;

  /// \brief Basic constructor which generates a timestamp and derives the message ID from it.
  NiftyLinkMessageContainer();

  /// \brief We need a copy constructor to register a Qt metatype.
  NiftyLinkMessageContainer(const NiftyLinkMessageContainer& another);

  /// \brief We might as well have operator equals if we have a copy constructor.
  /// This implies that we are considering this type as a Value Type.
  NiftyLinkMessageContainer& operator=(const NiftyLinkMessageContainer&);

  /// \brief Basic destructor.
  virtual ~NiftyLinkMessageContainer(void);

  /// \brief Returns the message ID.
  igtlUint64 GetNiftyLinkMessageId(void) const;

  /// \brief This function sets the OpenIGTLink message, which copies the smart pointer.
  void SetMessage(igtl::MessageBase::Pointer mp);

  /// \brief This function copies and returns the embedded OpenIGTLink message smart pointer.
  igtl::MessageBase::Pointer GetMessage() const;

  /// \brief Set the time arrived, which is copied into this object.
  void SetTimeArrived(const igtl::TimeStamp::Pointer &time);

  /// \brief Get the time arrived, which is copied out of this object, in nanoseconds since Unix Epoch.
  igtlUint64 GetTimeArrived() const;

  /// \brief Set the time received, which is copied into this object.
  void SetTimeReceived(const igtl::TimeStamp::Pointer& time);

  /// \brief Get the time received, which is copied out of this object, in nanoseconds since Unix Epoch.
  igtlUint64 GetTimeReceived() const;

  /// \brief Retrieves the time created, directly from the message, in nanoseconds since Unix Epoch.
  void GetTimeCreated(igtl::TimeStamp::Pointer& time) const;

  /// \brief Calculates the latency as difference between time created and time received.
  igtlUint64 GetLatency() const;

  /// \brief Set the host name of where the message came from.
  void SetSenderHostName(const QString &host);

  /// \brief Get the host name of where the message came from.
  QString GetSenderHostName() const;

  /// \brief Set the sender host's port number.
  void SetSenderPortNumber(const int& portNumber);

  /// \brief Get the sender host's port number.
  int GetSenderPortNumber() const;

  /// \brief Set the name of the owner class as the message is passed back and forth
  void SetOwnerName(const QString& str);

  /// \brief Method to check who owns the message at the moment
  QString GetOwnerName(void);

private:

  // Shallow copy, meaning that it copies pointer values.
  void ShallowCopy(const NiftyLinkMessageContainer& another);

  // Holds the actual message. All operations on the message should be done directly here.
  igtl::MessageBase::Pointer         m_Message;

  // To give the message a unique ID.
  igtlUint64                         m_Id;

  // To mark when the first byte of the message arrived at the socket
  igtlUint64                         m_TimeArrived;

  // To mark when the message was fully received
  igtlUint64                         m_TimeReceived;

  // To indicate which host/ip address the message came from.
  QString                            m_SenderHostName;

  // To indicate which port number the message came from.
  int                                m_SenderPortNumber;

  // We store an indicator of who 'owns' this message.
  QString                            m_OwnerName;
};

} // end namespace niftk

Q_DECLARE_METATYPE(niftk::NiftyLinkMessageContainer::Pointer);

#endif // NiftyLinkMessageContainer_h
