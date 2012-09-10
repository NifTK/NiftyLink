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

#ifndef NLTEST_DATA_H
#define NLTEST_DATA_H

#include "igtlMath.h"

static igtl::Matrix4x4 dummyTransformMatrix =
{{ -1, 0, 0, 50},
{  0, 0.15, 0.98, 0},
{ 0, 0.98, -0.15, 50},
{ 0, 0, 0, 1} };

#endif // NLTEST_DATA_H
