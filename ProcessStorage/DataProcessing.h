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

class DataProcessing
{
public:
	DataProcessing();
	~DataProcessing();
	void ClearExpiredInfo();
	void GetCurrent(SYSTEMTIME& st);
	DWORD ChangeSysTime2NumType(SYSTEMTIME& st);
	bool IsDuplication(const boost::uuids::uuid& ud);
	boost::uuids::uuid DataSupplement(const processInfo& info);
	void AddNew(const boost::uuids::uuid& ud, std::shared_ptr<processInfo> sp);

};

