/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef __NiftyLinkCommonWin32ExportHeader_h
#define __NiftyLinkCommonWin32ExportHeader_h

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


#endif  //__NiftyLinkCommonWin32ExportHeader_h
