/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: Monitor.h
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
#include <string>
#include <windows.h>
#include "ProcessInfoCache.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <set>
#include <map>
#include "LogInfo.h"

class DataProcessing
{
public:
	DataProcessing();
	~DataProcessing();
	void Init(LogInfo& log);
	void ClearExpiredInfo(unsigned int expireTime);
	void GetCurrent(DWORD& time);
	DWORD ChangeSysTime2NumType(SYSTEMTIME& st);
	bool IsDuplication(const boost::uuids::uuid& ud);
	boost::uuids::uuid DataSupplement(std::shared_ptr<processInfo> sp);
	void AddNew(const boost::uuids::uuid& ud, std::shared_ptr<processInfo> sp);
	void SetExiteTime(std::set<boost::uuids::uuid>& uuidSet);
	LogInfo m_log;
};

