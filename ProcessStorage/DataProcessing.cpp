
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

// uuid��ͬ
bool DataProcessing::IsDuplication(const boost::uuids::uuid& ud)
{
	return ProcessInfoCache::GetInstance()->IsExist(ud);
}

// �жϻ����������Ƿ��н������˳�������˳������˳�ʱ��Ϊ��ǰʱ��
void DataProcessing::SetExiteTime(std::set<boost::uuids::uuid>& uuidSet)
{
	std::map<boost::uuids::uuid, std::shared_ptr<processInfo>> cacheMap;
	ProcessInfoCache::GetInstance()->GetAll(cacheMap);
	for (auto& it : cacheMap)
	{
		string str = string("filename = ") + wstring2string(it.second->fileName) + string("  filepath = ") + wstring2string(it.second->filePath)
			+ string(" starttime = ") + std::to_string(it.second->startTime) + string( "pid = ") + std::to_string(it.second->pid);
		m_log.WriteLog(str);
		if (uuidSet.find(it.first) == uuidSet.end())  // �����������uuid�Ƿ��Ǳ��ֿ���uuid
		{
			DWORD nowTime;
			GetCurrent(nowTime);
			ProcessInfoCache::GetInstance()->SetExitTime(it.first, nowTime);
		}
	}
	// todo ����������
}
// ʹ��������������һ��uuid��֤Ψһ
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