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

QString resolveHostName(const QString &input)
{
	QHostInfo info = QHostInfo::fromName(input);
	return info.addresses().first().toString();
}

QString getLocalHostAddress(void)
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

void GetRandomTestMatrix(igtl::Matrix4x4& matrix)
{
  float position[3];
  float orientation[4];

  // random position
  static float phi = 0.0;
  position[0] = 50.0 * cos(phi);
  position[1] = 50.0 * sin(phi);
  position[2] = 50.0 * cos(phi);
  phi = phi + 0.2;

  // random orientation
  static float theta = 0.0;
  orientation[0]=0.0;
  orientation[1]=0.6666666666*cos(theta);
  orientation[2]=0.577350269189626;
  orientation[3]=0.6666666666*sin(theta);
  theta = theta + 0.1;

  //igtl::Matrix4x4 matrix;
  igtl::QuaternionToMatrix(orientation, matrix);

  matrix[0][3] = position[0];
  matrix[1][3] = position[1];
  matrix[2][3] = position[2];
  
  //igtl::PrintMatrix(matrix);
}
