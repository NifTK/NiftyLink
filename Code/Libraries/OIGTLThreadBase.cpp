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

#include "OIGTLThreadBase.h"

OIGTLThreadBase::OIGTLThreadBase(QObject *parent)
: QThread(parent)
{
	m_port           = -1;
    m_socketTimeout  = 50;
	m_running        = false;
	m_initialized    = false;
	m_mutex          = NULL;
	m_extSocket      = 0;

  m_timeouter.setInterval(m_socketTimeout);
  m_timeouter.setSingleShot(false);
  connect(this, SIGNAL(restartTimer(int )), &m_timeouter, SLOT(start(int )), Qt::QueuedConnection); 
  connect(this, SIGNAL(stopTimer()), &m_timeouter, SLOT(stop()), Qt::QueuedConnection); 
}

OIGTLThreadBase::~OIGTLThreadBase(void)
{
}
