/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkUtils.h"
#include <NiftyLinkTrackingDataMessageHelpers.h>
#include <NiftyLinkTransformMessageHelpers.h>
#include <NiftyLinkImageMessageHelpers.h>

#include <igtlTrackingDataMessage.h>
#include <igtlTransformMessage.h>
#include <igtlStringMessage.h>
#include <igtlStatusMessage.h>
#include <igtlImageMessage.h>

#include <QsLog.h>
#include <QsLogDest.h>

#include <QUrl>
#include <QHostInfo>
#include <QHostAddress>
#include <QStringList>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QNetworkSession>
#include <QDesktopServices>
#include <QDir>

#include <cmath>
#include <cassert>

namespace niftk
{

#if defined(_WIN32) && !defined(__CYGWIN__)
//-----------------------------------------------------------------------------
void InitializeWinTimers()
{
  // Use LoadLibrary used to load ntdll
  HINSTANCE hInstLibrary = LoadLibrary("ntdll.dll");

  if (hInstLibrary)
  {
    // Typedef functions to hold what is in the DLL
    FunctionPtr_SETRES _NtSetTimerResolution;
    FunctionPtr_GETRES _NtQueryTimerResolution;

    // the DLL is loaded and ready to go.
    _NtSetTimerResolution = (FunctionPtr_SETRES)GetProcAddress(hInstLibrary, "NtSetTimerResolution");
    _NtQueryTimerResolution = (FunctionPtr_GETRES)GetProcAddress(hInstLibrary, "NtQueryTimerResolution");

    if (_NtSetTimerResolution)
    {
      uint desiredResolution = 5000;
      bool setResolution = true;
      ULONG minResolution = 0;
      ULONG maxResolution = 0;
      ULONG currentResolution = 0;

      _NtQueryTimerResolution(&minResolution, &maxResolution, &currentResolution);
      QLOG_INFO() <<"Current Clock Resolution - Before: " << currentResolution;

      _NtSetTimerResolution(maxResolution, setResolution, &currentResolution);

      _NtQueryTimerResolution(&minResolution, &maxResolution, &currentResolution);
      QLOG_INFO() <<"Current Clock Resolution - After: " << currentResolution;
    }
  }
  else
  {
    QLOG_ERROR() << "Failed to load ntdll.dll to set timer resolution.";
  }
}
#endif

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
  assert(timeA);
  assert(timeB);

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
    stdDev += ((static_cast<double>(item)-mean)*(static_cast<double>(item)-mean));
  }
  if (list.size() > 1)
  {
    stdDev /= static_cast<double>(list.size() - 1);
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
bool IsCloseEnoughTo(double valueA, double valueB, double tolerance)
{
  return fabs(valueA - valueB) < tolerance;
}


//-----------------------------------------------------------------------------
bool IsCloseEnoughToZero(double valueA, double tolerance)
{
  return IsCloseEnoughTo(valueA, 0, tolerance);
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


//-----------------------------------------------------------------------------
QString GetTempDirectoryPath()
{
  return QDesktopServices::storageLocation(QDesktopServices::TempLocation);
}


//-----------------------------------------------------------------------------
QString GetWritableDirectoryPath(const QString& fileName)
{
  QString result;
  QDir directory;

  QString path = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
  directory.setPath(path);

  if (!directory.exists())
  {
    path = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
    directory.setPath(path);
  }
  if (!directory.exists())
  {
    path = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    directory.setPath(path);
  }
  if (!directory.exists())
  {
    path = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    directory.setPath(path);
  }
  if (!directory.exists())
  {
    path = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    directory.setPath(path);
  }
  if (!directory.exists())
  {
    path = QDir::currentPath();
    directory.setPath(path);
  }
  if (!directory.exists())
  {
    path = "";
  }

  result = path;

  if (fileName.length() > 0)
  {
    if (result.length() > 0)
    {
      result = result + QDir::separator() + fileName;
    }
    else
    {
      result = fileName;
    }
  }
  return result;
}


//-----------------------------------------------------------------------------
QString AppendPathSeparator(const QString& path)
{
  return path.right(1) == "/" ? path : path + "/";
}


//-----------------------------------------------------------------------------
void ExtractTextBasedMessage(const NiftyLinkMessageContainer::Pointer& message, QString& output)
{
  if (message.data() != NULL)
  {
    igtl::MessageBase::Pointer msg = dynamic_cast<igtl::MessageBase*>(message->GetMessage().GetPointer());
    if (msg.IsNotNull())
    {
      if (QString(msg->GetDeviceType()) == QString("STRING"))
      {
        igtl::StringMessage::Pointer m = dynamic_cast<igtl::StringMessage*>(msg.GetPointer());
        output = QString(m->GetString());
      }
      if (QString(msg->GetDeviceType()) == QString("STATUS"))
      {
        igtl::StatusMessage::Pointer m = dynamic_cast<igtl::StatusMessage*>(msg.GetPointer());
        output = QString(m->GetStatusString());
      }
      if (QString(msg->GetDeviceType()) == QString("TRANSFORM"))
      {
        igtl::TransformMessage::Pointer m = dynamic_cast<igtl::TransformMessage*>(msg.GetPointer());
        output = GetMatrixAsString(m);
      }
      if (QString(msg->GetDeviceType()) == QString("TDATA"))
      {
        igtl::TrackingDataMessage::Pointer m = dynamic_cast<igtl::TrackingDataMessage*>(msg.GetPointer());
        output = GetMatrixAsString(m,0);
      }
    }
  }
}


//-----------------------------------------------------------------------------
void ExtractImageMessage(const NiftyLinkMessageContainer::Pointer& message, QImage& image)
{
  if (message.data() != NULL)
  {
    igtl::ImageMessage::Pointer msg = dynamic_cast<igtl::ImageMessage*>(message->GetMessage().GetPointer());
    if (msg.IsNotNull())
    {
      GetQImage(msg, image);
    }
  }
}


//-----------------------------------------------------------------------------
bool IsKeepAlive(const igtl::MessageBase::Pointer& message)
{
  bool isKeepAlive = false;
  igtl::StatusMessage::Pointer msg = dynamic_cast<igtl::StatusMessage*>(message.GetPointer());
  if (msg.IsNotNull())
  {
    if (msg->GetCode() == igtl::StatusMessage::STATUS_OK)
    {
      isKeepAlive = true;
    }
  }
  return isKeepAlive;
}


//-----------------------------------------------------------------------------
bool IsStatsRequest(const igtl::MessageBase::Pointer& message)
{
  bool isStats = false;
  igtl::StringMessage::Pointer msg = dynamic_cast<igtl::StringMessage*>(message.GetPointer());
  if (msg.IsNotNull())
  {
    if (msg->GetString() == std::string("STATS"))
    {
      isStats = true;
    }
  }
  return isStats;
}


//-----------------------------------------------------------------------------
bool IsCloseEnoughTo(const igtl::Matrix4x4& a, const igtl::Matrix4x4& b, double tolerance)
{
  for (unsigned int r = 0; r < 4; r++)
  {
    for (unsigned int c = 0; c < 4; c++)
    {
      if (!IsCloseEnoughTo(a[r][c], b[r][c], tolerance))
      {
        return false;
      }
    }
  }
  return true;
}

} // end namespace niftk
