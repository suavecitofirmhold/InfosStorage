
//#include <objbase.h>
#include <sstream>
#include "DataProcessing.h"
#include "Tool.h"

DataProcessing::DataProcessing()
{
	
}


DataProcessing::~DataProcessing()
{
}

void DataProcessing::ClearExpiredInfo()
{
	ProcessInfoCache::GetInstance()->EraseExpiredData();
}

void DataProcessing::GetCurrent(SYSTEMTIME& st)
{
	GetSystemTime(&st);
}

DWORD DataProcessing::ChangeSysTime2NumType(SYSTEMTIME& st)
{
	DWORD numType =
		st.wHour * 3600000   // 60 * 60 * 1000
		+ st.wMinute * 6000  // 60 * 1000
		+ st.wSecond * 1000
		+ st.wMilliseconds;
	return numType;
}

bool DataProcessing::IsDuplication(const boost::uuids::uuid& ud)
{
	return ProcessInfoCache::GetInstance()->IsExist(ud);
}

boost::uuids::uuid DataProcessing::DataSupplement(const processInfo& info)
{
	wchar_t temp[64];
	std::wstring arr = info.fileName + std::wstring(_itow(info.pid, temp, 10))
		+ std::wstring(_itow(info.startTime, temp, 10));
	return GenerateStrBasedUuid(arr);
}

void DataProcessing::AddNew(const boost::uuids::uuid& ud, std::shared_ptr<processInfo> sp)
{
	ProcessInfoCache::GetInstance()->Push(ud, sp);
}
