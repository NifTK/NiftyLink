/*=============================================================================

 NiftyLink:  A software library to facilitate communication over OpenIGTLink.

             http://cmic.cs.ucl.ac.uk/
             http://www.ucl.ac.uk/

 Copyright (c) UCL : See LICENSE.txt in the top level directory for details.

 Last Changed      : $Date: 2010-05-25 17:02:50 +0100 (Tue, 25 May 2010) $
 Revision          : $Revision: 3300 $
 Last modified by  : $Author: mjc $

 Original author   : m.clarkson@ucl.ac.uk

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 ============================================================================*/

#include "OIGTLProcessBase.h"

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

OIGTLProcessBase::~OIGTLProcessBase(void)
{
}
