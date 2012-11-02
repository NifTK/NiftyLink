/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NIFTYLINKCOMMONUTILS_H
#define NIFTYLINKCOMMONUTILS_H

#include <QString>
#include <QUrl>
#include <QHostInfo>
#include <QHostAddress>
#include <QStringList>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QNetworkSession>

#include "igtlPolyDataMessage.h"
#include "igtlTrajectoryMessage.h"
#include "igtlLightObject.h"
#include "igtlObjectFactory.h"

#include <cmath>
#include <map>
#include <string>
#include "igtlMath.h"

#include "NiftyLinkConfig.h"
#include "NiftyLinkCommonWin32ExportHeader.h"

#include "OIGTLMessage.h"

#include "QsLog.h"
#include "QsLogDest.h"

enum MessageTypes 
{
  NONDEFINED,
  BIND,
  GET_BIND,
  STT_BIND,
  STP_BIND,
  RTS_BIND,
  COLORTABLE,
  GET_COLORT,
  STT_COLORT,
  STP_COLORT,
  RTS_COLORT,
  IMAGE,
  GET_IMAGE,
  STT_IMAGE,
  STP_IMAGE,
  RTS_IMAGE,
  IMGMETA,
  GET_IMGMETA,
  STT_IMGMETA,
  STP_IMGMETA,
  RTS_IMGMETA,
  LBMETA,
  GET_LBMETA,
  STT_LBMETA,
  STP_LBMETA,
  RTS_LBMETA,
  NDARRAY,
  GET_NDARRAY,
  STT_NDARRAY,
  STP_NDARRAY,
  RTS_NDARRAY,
  POINTMSG,
  GET_POINTMSG,
  STT_POINTMSG,
  STP_POINTMSG,
  RTS_POINTMSG,
  POLYDATA,
  GET_POLYDATA,
  STT_POLYDATA,
  STP_POLYDATA,
  RTS_POLYDATA,
  POSITION,
  GET_POSITION,
  STT_POSITION,
  STP_POSITION,
  RTS_POSITION,
  QTDATA,
  GET_QTDATA,
  STT_QTDATA,
  STP_QTDATA,
  RTS_QTDATA,
  SENSOR,
  GET_SENSOR,
  STT_SENSOR,
  STP_SENSOR,
  RTS_SENSOR,
  STATUS,
  GET_STATUS,
  STT_STATUS,
  STP_STATUS,
  RTS_STATUS,
  STRING,
  GET_STRING,
  STT_STRING,
  STP_STRING,
  RTS_STRING,
  TDATA,
  GET_TDATA,
  STT_TDATA,
  STP_TDATA,
  RTS_TDATA,
  TRAJ,
  GET_TRAJ,
  STT_TRAJ,
  STP_TRAJ,
  RTS_TRAJ,
  TRANSFORM,
  GET_TRANS,
  STT_TRANS,
  STP_TRANS,
  RTS_TRANS
};

// Map to associate the strings with the enum values
typedef std::map<std::string, MessageTypes> mapStrMsgType;
static mapStrMsgType strMsgTypes; 

/// \brief Initialises the types map.
extern "C" NIFTYLINKCOMMON_WINEXPORT void InitMessageTypes(mapStrMsgType &types);

/// \brief Simple function which validates if an IP address is valid or not
extern "C++" NIFTYLINKCOMMON_WINEXPORT bool validateIp(const QString &inputIP);

/// \brief This function which returns the local IP address
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString getLocalHostAddress(void);

/// \brief This function resolves a given hostname to IP
extern "C++" NIFTYLINKCOMMON_WINEXPORT QString resolveHostName(const QString &input);

/// \brief This function creates a random 4x4 test matrix 
extern "C++" NIFTYLINKCOMMON_WINEXPORT void CreateRandomTransformMatrix(igtl::Matrix4x4& matrix);

#endif
