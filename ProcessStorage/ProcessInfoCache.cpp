#include "ProcessInfoCache.hpp"

ProcessInfoCache::ProcessInfoCache()
{
	log.Init("F:\\Debug_x64");
}

ProcessInfoCache* ProcessInfoCache::GetInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new ProcessInfoCache();
	}
	return m_instance;
}

void ProcessInfoCache::Push(std::string guid, processInfo& pInfo)
{
	//std::pair<std::map<std::string, processInfo>, bool> ret;
	//ret = 
		m_pInfoMap.insert(std::make_pair(guid, pInfo));
	//if (!ret.second)
	//{
	//	// todo
	//	log.WriteLog("insert failed");
	//}
}

void ProcessInfoCache::Remove(std::string guid)
{
	auto it = m_pInfoMap.find(guid);
	if (it != m_pInfoMap.end())
	{
		m_pInfoMap.erase(it);
	}
	else
	{
		// todo
		log.WriteLog("erase failed");
	}
}

void ProcessInfoCache::EraseExpiredData()
{
	for (auto it = m_pInfoMap.begin(); it != m_pInfoMap.end();)
	{
		if (TRUE == it->second.exitFlag)
		{
			m_pInfoMap.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void ProcessInfoCache::Modify(std::string& guid, processInfo& pInfo)
{
	m_pInfoMap[guid] = pInfo;
}

void ProcessInfoCache::ModifyExitFlag(std::string& guid, BOOL flag)
{
	m_pInfoMap[guid].exitFlag = flag;
}

void ProcessInfoCache::SetExitTime(std::string& guid)
{

}
processInfo ProcessInfoCache::GetProcessInfo(std::string& guid)
{
	return m_pInfoMap[guid];
}

void ProcessInfoCache::GetAll(std::map<std::string, processInfo>& outMap)
{
	outMap = m_pInfoMap;
}