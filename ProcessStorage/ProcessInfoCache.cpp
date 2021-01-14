#include "ProcessInfoCache.h"

ProcessInfoCache* ProcessInfoCache::m_instance = nullptr;
std::mutex ProcessInfoCache::m_mutex;
ProcessInfoCache::ProcessInfoCache()
{
	//log.Init("F:\\Debug_x64");
}

ProcessInfoCache::~ProcessInfoCache()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}
ProcessInfoCache* ProcessInfoCache::GetInstance()
{
	if (nullptr == m_instance)
	{
		if (nullptr == m_instance)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_instance = new ProcessInfoCache();
		}
	}
	return m_instance;
}

void ProcessInfoCache::Push(boost::uuids::uuid uuid, const processInfo& pInfo)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	std::shared_ptr<processInfo> sp(new processInfo(pInfo));

	m_pInfoMap.insert(std::make_pair(uuid, sp));
}

void ProcessInfoCache::Remove(boost::uuids::uuid uuid)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	auto it = m_pInfoMap.find(uuid);
	if (it != m_pInfoMap.end())
	{
		m_pInfoMap.erase(it);
	}
}

void ProcessInfoCache::EraseExpiredData()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	for (auto it = m_pInfoMap.begin(); it != m_pInfoMap.end();)
	{
		if (0 != it->second->exitTime)
		{
			m_pInfoMap.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void ProcessInfoCache::Modify(const boost::uuids::uuid& uuid, const processInfo& pInfo)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_pInfoMap[uuid].reset(new processInfo(pInfo));
}

void ProcessInfoCache::SetExitTime(const boost::uuids::uuid& uuid, DWORD time)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_pInfoMap[uuid]->exitTime = time;
}
std::shared_ptr<processInfo> ProcessInfoCache::GetProcessInfo(const boost::uuids::uuid& uuid)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_pInfoMap[uuid];
}

void ProcessInfoCache::GetAll(std::map<boost::uuids::uuid, std::shared_ptr<processInfo>>& outMap)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	outMap = m_pInfoMap;
}

bool ProcessInfoCache::IsExist(const boost::uuids::uuid& ui)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_pInfoMap.find(ui) == m_pInfoMap.end())
	{
		return false;
	}
	return true;
}