/*=============================================================================
NiftyLink: A software library to facilitate communication over OpenIGTLink.

Copyright (c) University College London (UCL). All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

See LICENSE.txt in the top level directory for details.
=============================================================================*/

#ifndef NiftyLinkSenderProcessTester_h
#define NiftyLinkSenderProcessTester_h

#include <NiftyLinkSenderProcess.h>

/**
* \class NiftyLinkSenderProcessTester
* \brief Class derived from NiftyLinkSenderProcess to enable unit testing.
*/
class NIFTYLINKCOMMON_WINEXPORT NiftyLinkSenderProcessTester : public NiftyLinkSenderProcess
{
  Q_OBJECT

  friend class TestSenderProcess_General;

  NiftyLinkSenderProcessTester(QObject *parent = 0)
    : NiftyLinkSenderProcess(parent) {}
};

#endif // NiftyLinkSenderProcessTester_h
