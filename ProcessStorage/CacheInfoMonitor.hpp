/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: CacheInfoMonitor.hpp
Abstract:
主要用于计算缓存的一些指标监控，如命中率、文件大小等。
依赖ProcessInfoCache的缓存资源用于计算
Remarks:
Revision History:
2020-01-13   陈鑫逾阳    创建
*/
#pragma once
class CacheInfoMonitor
{
public:
	CacheInfoMonitor();
	~CacheInfoMonitor();
	float CalcHitRate();
	unsigned int GetInfoSize();
private:
	float hitRate;
	unsigned int infoSize;
};

