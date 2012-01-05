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
#ifndef __NIFTYLINKCOMMONWIN32EXPORTHEADER_H
#define __NIFTYLINKCOMMONWIN32EXPORTHEADER_H

#include "NiftyLinkConfig.h"

#if (defined(_WIN32) || defined(WIN32)) && !defined(LINK_STATIC)
  #ifdef NIFTYLINKCOMMON_WINDOWS_EXPORT
    #define NIFTYLINKCOMMON_WINEXPORT __declspec(dllexport)
  #else
    #define NIFTYLINKCOMMON_WINEXPORT __declspec(dllimport)
  #endif  /* NIFTYLINKCOMMON_WINEXPORT */
#else
/* linux/mac needs nothing */
  #define NIFTYLINKCOMMON_WINEXPORT
#endif


#endif  //__NIFTYLINKCOMMONWIN32EXPORTHEADER_H
