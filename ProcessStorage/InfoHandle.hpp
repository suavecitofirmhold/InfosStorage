/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: CacheInfoMonitor.hpp
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
#include <string>
#include <windows.h>
#include "ProcessInfoCache.hpp"

class InfoHandle
{
public:
	InfoHandle();
	~InfoHandle();
	void SetExitProcess(std::string& guid);
	void ClearExpiredInfo();
	std::string Change2Md5(processInfo& p);
	void GetCurrent(SYSTEMTIME& st);
	DWORD ChangeSysTime2NumType(SYSTEMTIME& st);
	void RemoveDuplication();
	std::string DataSupplement();
};

