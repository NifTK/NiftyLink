/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include "NiftyLinkProcessBase.h"

//-----------------------------------------------------------------------------
NiftyLinkProcessBase::NiftyLinkProcessBase(QObject *parent)
: QObject(parent)
{
  m_Port           = -1;
  m_SocketTimeout  = 50;
  m_Running        = false;
  m_Active         = false;
  m_Initialized    = false;
  m_Mutex          = NULL;
  m_TimeOuter      = NULL;
  m_ExtSocket      = 0;
  m_MessageCounter = 0;
}


//-----------------------------------------------------------------------------
NiftyLinkProcessBase::~NiftyLinkProcessBase(void)
{
}
