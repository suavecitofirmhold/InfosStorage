/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: Monitor.h
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <mutex>
//#include <boost/thread.hpp>
//#include <boost/thread/lock_factories.hpp>
//#include <boost/thread/mutex.hpp>
#include "Log.h"

struct processInfo {
	std::wstring fileName;
	std::wstring szExeFile;
	std::wstring userName;
	std::string mdFive;
	std::string cmdLine;
	//DWORD th32ProcessID;
	DWORD pid;
	DWORD ppid;
	DWORD startTime;
	DWORD exitTime;
	processInfo():fileName(L""), szExeFile(L""), userName(L""), exitTime(0){};
	processInfo(const processInfo& info)
	{
		fileName = info.fileName;
		szExeFile = info.szExeFile;
		userName = info.userName;
		mdFive = info.mdFive;
		cmdLine = info.cmdLine;
		pid = info.pid;
		ppid = info.ppid;
		startTime = info.startTime;
		exitTime = info.exitTime;
	}
};

class ProcessInfoCache
{
public:
	static ProcessInfoCache* GetInstance();
	void Push(boost::uuids::uuid uuid, const processInfo& pInfo);
	void Remove(boost::uuids::uuid uuid);
	void EraseExpiredData();
	void Modify(const boost::uuids::uuid& uuid, const processInfo& pInfo);
	void ModifyExitTime(const boost::uuids::uuid& uuid, DWORD time);
	void SetExitTime(const boost::uuids::uuid& uuid, DWORD time);
	std::shared_ptr<processInfo> GetProcessInfo(const boost::uuids::uuid& uuid);
	void GetAll(std::map<boost::uuids::uuid, std::shared_ptr<processInfo>>& outMap);
	bool IsExist(const boost::uuids::uuid& ui);

	unsigned int GetInfoSize()
	{
		return m_pInfoMap.size();
	}
private:
	ProcessInfoCache();
	~ProcessInfoCache();
public:
	static std::mutex m_mutex;
private:
	std::map<boost::uuids::uuid, std::shared_ptr<processInfo>> m_pInfoMap;
	static ProcessInfoCache* m_instance;
	
};

