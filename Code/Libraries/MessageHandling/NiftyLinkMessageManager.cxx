/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#include "NiftyLinkMessageManager.h"

#include <QMutexLocker>

namespace niftk
{

//-----------------------------------------------------------------------------
NiftyLinkMessageManager::NiftyLinkMessageManager(QObject *parent)
{
}


//-----------------------------------------------------------------------------
NiftyLinkMessageManager::~NiftyLinkMessageManager()
{
}


//-----------------------------------------------------------------------------
void NiftyLinkMessageManager::InsertContainer(int portNumber, NiftyLinkMessageContainer::Pointer container)
{
  QMutexLocker locker(&m_Mutex);
  m_Data.insert(portNumber, container);
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer NiftyLinkMessageManager::GetContainer(int portNumber)
{
  QMutexLocker locker(&m_Mutex);
  if (m_Data.isEmpty() || !m_Data.contains(portNumber))
  {
    return NiftyLinkMessageContainer::Pointer(NULL);
  }
  else
  {
    return m_Data.value(portNumber);
  }
}


//-----------------------------------------------------------------------------
NiftyLinkMessageContainer::Pointer NiftyLinkMessageManager::GetContainer()
{
  QMutexLocker locker(&m_Mutex);
  if (m_Data.isEmpty())
  {
    return NiftyLinkMessageContainer::Pointer(NULL);
  }
  else
  {
    return m_Data.value(m_Data.keys().first());
  }
}

} // end namespace niftk
