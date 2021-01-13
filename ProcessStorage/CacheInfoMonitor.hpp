/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: CacheInfoMonitor.hpp
Abstract:
��Ҫ���ڼ��㻺���һЩָ���أ��������ʡ��ļ���С�ȡ�
����ProcessInfoCache�Ļ�����Դ���ڼ���
Remarks:
Revision History:
2020-01-13   ��������    ����
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

