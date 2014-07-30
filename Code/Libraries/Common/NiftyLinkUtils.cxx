/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkUtils.h"

#include <igtlTrackingDataMessage.h>

#include <QUrl>
#include <QHostInfo>
#include <QHostAddress>
#include <QStringList>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QNetworkSession>

#include <cmath>
#include <cassert>

namespace niftk
{

//-----------------------------------------------------------------------------
bool ValidateIp(const QString &inputIP)
{
  QStringList nums = inputIP.split(".");

  if (nums.count() != 4)
  {
    return false;
  }

  for (int i = 0; i < 4; i++)
  {
    bool ok = false;
    int frag = nums.at(i).toInt(&ok, 10);

    if (!ok)
    {
      return false;
    }
    else if (frag < 0 || frag > 255)
    {
      return false;
    }
  }

  return true;
}


//-----------------------------------------------------------------------------
QString ResolveHostName(const QString &input)
{
  if (ValidateIp(input))
  {
    return input;
  }

  QHostInfo info = QHostInfo::fromName(input);
  QList<QHostAddress> addresses = info.addresses();

  for (int i = 0; i < addresses.count(); i++)
  {
    if ( ValidateIp(addresses.at(i).toString()) == true )
    {
      return addresses.at(i).toString();
    }
  }

  return QString("UNKNOWN");

}


//-----------------------------------------------------------------------------
QString GetLocalHostAddress(void)
{
  QNetworkConfigurationManager mgr;
  QNetworkConfiguration nconfig = mgr.defaultConfiguration();
  QNetworkSession session ( nconfig );
  QNetworkInterface ninter = session.interface();

  // this provides more than one ip addresses
  QList<QNetworkAddressEntry> laddr = ninter.addressEntries();
  for ( QList<QNetworkAddressEntry> ::const_iterator it = laddr.begin(); it != laddr.end(); ++it )
  {
    QString ipstr = it->ip().toString();
    if (ValidateIp(ipstr))
    {
      // Need to return the one which is valid and not 'localhost'
      if (ipstr != QString("127.0.0.1"))
      {
        return ipstr;
      }
    }
  }

  return QString("UNKNOWN");
}


//-----------------------------------------------------------------------------
void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix)
{
  float position[3];
  float orientation[4];

  // random position
  static float phi = 0.2;
  position[0] = 50.0 * cos(phi);
  position[1] = 50.0 * sin(phi);
  position[2] = 50.0 * cos(phi);
  phi = phi + 0.2;

  // random orientation
  static float theta = 0.0;
  orientation[0] = 0.0;
  orientation[1] = 0.6666666666 * cos(theta);
  orientation[2] = 0.577350269189626;
  orientation[3] = 0.6666666666 * sin(theta);
  theta = theta + 0.1;

  //igtl::Matrix4x4 matrix;
  igtl::QuaternionToMatrix(orientation, matrix);

  matrix[0][3] = position[0];
  matrix[1][3] = position[1];
  matrix[2][3] = position[2];

  //igtl::PrintMatrix(matrix);
}


//-----------------------------------------------------------------------------
igtlUint64 GetDifferenceInNanoSeconds(igtl::TimeStamp* timeA, igtl::TimeStamp* timeB)
{
  igtlUint64 a = timeA->GetTimeStampInNanoseconds();
  igtlUint64 b = timeB->GetTimeStampInNanoseconds();
  igtlUint64 d;

  if (a > b)
  {
    d = a - b;
  }
  else if (b > a)
  {
    d = b - a;
  }
  else
  {
    d = 0;
  }

  return d;
}


//-----------------------------------------------------------------------------
double CalculateMean(const QList<igtlUint64>& list)
{
  double mean = 0;
  foreach(igtlUint64 item, list)
  {
    mean += static_cast<double>(item);
  }
  if (list.size() > 1)
  {
    mean /= static_cast<double>(list.size());
  }
  return mean;
}


//-----------------------------------------------------------------------------
double CalculateStdDev(const QList<igtlUint64>& list)
{
  double stdDev = 0;
  double mean = CalculateMean(list);
  foreach(igtlUint64 item, list)
  {
    stdDev += ((item-mean)*(item-mean));
  }
  if (list.size() > 1)
  {
    stdDev /= static_cast<double>(list.size());
  }
  stdDev = sqrt(stdDev);
  return stdDev;
}


//-----------------------------------------------------------------------------
igtlUint64 CalculateMax(const QList<igtlUint64>& list)
{
  igtlUint64 max = 0;
  foreach(igtlUint64 item, list)
  {
    if (item > max)
    {
      max = item;
    }
  }
  return max;
}



//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer CreateTestTrackingDataMessage(int matricesPerMessage)
{
  igtl::Matrix4x4 matrix;

  igtl::TrackingDataMessage::Pointer msg = igtl::TrackingDataMessage::New();
  msg->SetDeviceName("NiftyLinkUtils");

  for (int i = 0; i < matricesPerMessage; i++)
  {
    niftk::CreateRandomTransformMatrix(matrix);

    igtl::TrackingDataElement::Pointer element = igtl::TrackingDataElement::New();
    element->SetMatrix(matrix);

    msg->AddTrackingDataElement(element);
  }

  igtl::TimeStamp::Pointer timeCreated = igtl::TimeStamp::New();
  timeCreated->GetTime();

  msg->SetTimeStamp(timeCreated);
  msg->Pack();

  NiftyLinkMessageContainer::Pointer m = (NiftyLinkMessageContainer::Pointer(new NiftyLinkMessageContainer()));
  m->SetMessage(msg.GetPointer());
  m->SetOwnerName("CreateTestTrackingDataMessage");
  m->SetSenderHostName("123.456.789.012");
  m->SetSenderPortNumber(1234);

  return m;
}


//-----------------------------------------------------------------------------
void CopyMatrix(double *input, igtl::Matrix4x4& output)
{
  assert(input);

  for (unsigned int r = 0; r < 4; r++)
  {
    for (unsigned int c = 0; c < 4; c++)
    {
      output[r][c] = input[r*4+c];
    }
  }
}

} // end namespace niftk

