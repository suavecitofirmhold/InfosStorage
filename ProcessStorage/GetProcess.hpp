/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: CacheInfoMonitor.hpp
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
#include "GetProcess.hpp"

class GetProcess {
public:
	bool GetAllProcessInfo();
	bool GetProcessInfo();
	bool Init();


	void DuplicationFind();
	void AddNew();
	void SetExitProcess();
	void ClearExpiredInfo();
};