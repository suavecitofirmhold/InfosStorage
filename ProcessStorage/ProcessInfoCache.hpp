/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: CacheInfoMonitor.hpp
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "LogManager.hpp"

struct processInfo {
	std::wstring fileName;
	std::wstring szExeFile;
	std::wstring userName;
	std::string mdFive;
	std::string cmdLine;
	DWORD th32ProcessID;
	DWORD pid;
	DWORD ppid;
	DWORD startTime;
	DWORD exitTime;
	BOOL exitFlag;
};

class ProcessInfoCache
{
public:
	static ProcessInfoCache* GetInstance();
	void Push(std::string guid, processInfo& pInfo);
	void Remove(std::string guid);
	void EraseExpiredData();
	void Modify(std::string& guid, processInfo& pInfo);
	void ModifyExitFlag(std::string& guid, BOOL flag);
	void SetExitTime(std::string& guid);
	processInfo GetProcessInfo(std::string& guid);
	void GetAll(std::map<std::string, processInfo>& outMap);
	unsigned int GetInfoSize()
	{
		return m_pInfoMap.size();
	}
private:
	ProcessInfoCache();
	~ProcessInfoCache();
private:
	static std::map<std::string, processInfo> m_pInfoMap;
	static ProcessInfoCache* m_instance;
	LogManager log;
};

