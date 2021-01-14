#include "Monitor.h"
#include "ProcessInfoCache.h"


CacheInfoMonitor::CacheInfoMonitor():hitRate(0.0),infoSize(0)
{
}


CacheInfoMonitor::~CacheInfoMonitor()
{
}

float CacheInfoMonitor::CalcHitRate() 
{
	// todo
	return 0.1;
}

unsigned int CacheInfoMonitor::GetInfoSize() 
{
	return ProcessInfoCache::GetInstance()->GetInfoSize();
}