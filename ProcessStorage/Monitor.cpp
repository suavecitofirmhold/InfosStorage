#include "Monitor.h"
#include "ProcessInfoCache.h"


Monitor::Monitor():hitRate(0.0),infoSize(0)
{
}


Monitor::~Monitor()
{
}

float Monitor::CalcHitRate(int hitCount, int totalNum) 
{
	if (totalNum == 0)
	{
		return 1;
	}
	return float(hitCount) / float(totalNum);
}

unsigned int Monitor::GetInfoSize() 
{
	unsigned int totalCount = ProcessInfoCache::GetInstance()->GetCount();
	int structSize = sizeof(processInfo);
	return totalCount * structSize;
}