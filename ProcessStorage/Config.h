/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: Monitor.h
Abstract:

Remarks:
Revision History:
2020-01-13   ≥¬ˆŒ”‚—Ù    ¥¥Ω®
*/
#pragma once
class Config
{
public:
	Config();
	~Config();
	unsigned int GetExpireTime();
	unsigned int GetRefreshTime();
	void Init();
private:
	unsigned int m_expire;
	unsigned int m_refresh;
};

