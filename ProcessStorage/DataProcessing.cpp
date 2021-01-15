
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

void DataProcessing::ClearExpiredInfo(unsigned int expireTime)
{
	DWORD time;
	GetCurrent(time);
	ProcessInfoCache::GetInstance()->EraseExpiredData(expireTime, time);
}

void DataProcessing::GetCurrent(DWORD& time)
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	time = ChangeSysTime2NumType(st);
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

// uuid相同
bool DataProcessing::IsDuplication(const boost::uuids::uuid& ud)
{
	return ProcessInfoCache::GetInstance()->IsExist(ud);
}

// 判断缓存数据中是否有进程已退出，如果退出则标记退出时间为当前时间
void DataProcessing::SetExiteTime(std::set<boost::uuids::uuid>& uuidSet)
{
	std::map<boost::uuids::uuid, std::shared_ptr<processInfo>> cacheMap;
	ProcessInfoCache::GetInstance()->GetAll(cacheMap);
	for (auto& it : cacheMap)
	{
		string str = string("filename = ") + wstring2string(it.second->fileName) + string("  filepath = ") + wstring2string(it.second->filePath)
			+ string(" starttime = ") + std::to_string(it.second->startTime) + string( "pid = ") + std::to_string(it.second->pid);
		m_log.WriteLog(str);
		if (uuidSet.find(it.first) == uuidSet.end())  // 遍历缓存进程uuid是否是本轮快照uuid
		{
			DWORD nowTime;
			GetCurrent(nowTime);
			ProcessInfoCache::GetInstance()->SetExitTime(it.first, nowTime);
		}
	}
	// todo 监视命中率
}
// 使用所有属性生成一个uuid保证唯一
boost::uuids::uuid DataProcessing::DataSupplement(std::shared_ptr<processInfo> sp)
{
	wchar_t temp[64];
	std::wstring arr = sp->fileName + sp->filePath + sp->userName
		+ std::wstring(_itow(sp->pid, temp, 10)) + std::wstring(_itow(sp->exitTime, temp, 10))
		+ std::wstring(_itow(sp->startTime, temp, 10))
		+ string2wstring(sp->mdFive) + string2wstring(sp->cmdLine)
		+ std::wstring(_itow(sp->ppid, temp, 10));
	return GenerateStrBasedUuid(arr);
}

void DataProcessing::AddNew(const boost::uuids::uuid& ud, std::shared_ptr<processInfo> sp)
{
	ProcessInfoCache::GetInstance()->Push(ud, sp);
}

void DataProcessing::Init(LogInfo& log)
{
	m_log = log;
}