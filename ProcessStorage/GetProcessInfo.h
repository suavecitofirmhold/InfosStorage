/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: Monitor.h
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include "DataProcessing.h"
using std::wstring;

class GetProcessInfo {
public:
	void Init(LogInfo& log, const unsigned int exp);
	BOOL GetProcessList();
	LPWSTR GetProcessUserName(DWORD pid);
	//void printError(TCHAR* msg);
	wstring DosDevicePath2LogicalPath(LPCTSTR lpszDosPath);
	LogInfo m_log;
	unsigned int m_expireTime;
private:
	DataProcessing dp;
};