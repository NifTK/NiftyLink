#include "OIGTLThreadBase.h"

OIGTLThreadBase::OIGTLThreadBase(QObject *parent)
: QThread(parent)
{
	m_port        = -1;
	m_running     = false;
	m_initialized = false;
	m_mutex       = NULL;
	m_extSocket   = 0;
}

OIGTLThreadBase::~OIGTLThreadBase(void)
{
}