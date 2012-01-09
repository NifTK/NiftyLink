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

#include "NiftyLinkUtils.h"

bool validateIp(const QString &inputIP)
{
	if (inputIP == QString("localhost"))
		return true;

	QStringList nums = inputIP.split(".");

	if (nums.count() !=4)
		return false;

	for (int i = 0; i < 4; i++)
	{
		bool ok = false;
		int frag = nums.at(i).toInt(&ok, 10);
		
		if (!ok) return false;
		else if (frag < 0 || frag > 255) return false;
	}
	
	return true;
}

static QString getLocalHostAddress(void)
{
	QHostInfo info;
	
	if (!info.addresses().isEmpty()) 
	{
		// use the first IP address
		return info.addresses().first().toString();
	}
	else
	{
		QString empty;
		empty.clear();
		return empty;
	}
}

int NiftyLinkDummyFunction1()
{
  return 0;
}