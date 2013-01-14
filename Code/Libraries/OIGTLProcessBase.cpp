/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "OIGTLProcessBase.h"

//-----------------------------------------------------------------------------
OIGTLProcessBase::OIGTLProcessBase(QObject *parent)
: QObject(parent)
{
  m_port           = -1;
  m_socketTimeout  = 50;
  m_running        = false;
  m_active         = false;
  m_initialized    = false;
  m_mutex          = NULL;
  m_timeouter      = NULL;
  m_extSocket      = 0;
  m_messageCounter = 0;
}


//-----------------------------------------------------------------------------
OIGTLProcessBase::~OIGTLProcessBase(void)
{
}
