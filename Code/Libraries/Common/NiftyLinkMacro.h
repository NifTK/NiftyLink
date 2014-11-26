/*=============================================================================
  NiftyLink:  A software library to facilitate communication over OpenIGTLink.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.
=============================================================================*/
#ifndef NiftyLinkMacro_h
#define NiftyLinkMacro_h

#include <sstream>
#include <stdexcept>

namespace niftk
{

#define NiftyLinkStdExceptionMacro(classname, message) \
  std::stringstream oss; \
  oss << "ERROR: In " << __FILE__ << ", line " << __LINE__ << ":" message << "\n"; \
  throw classname(oss.str()); \

} // end namespace niftk
#endif
