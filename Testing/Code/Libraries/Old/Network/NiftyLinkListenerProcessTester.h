/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkListenerProcessTester_h
#define NiftyLinkListenerProcessTester_h

#include <NiftyLinkListenerProcess.h>

/**
* \class NiftyLinkListenerProcessTester
* \brief Class derived from NiftyLinkListenerProcess to enable unit testing.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkListenerProcessTester : public NiftyLinkListenerProcess
{
  Q_OBJECT

  friend class TestListenerProcess_General;

  NiftyLinkListenerProcessTester(QObject *parent = 0)
    : NiftyLinkListenerProcess(parent) {}
};

#endif // NiftyLinkListenerProcessTester_h
