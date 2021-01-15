/*
Copyright (c) Onescorpion Technologies Co.,Ltd. All rights reserved.
Module Name: Monitor.h
Abstract:
��Ҫ���ڼ��㻺���һЩָ���أ��������ʡ��ļ���С�ȡ�
����ProcessInfoCache�Ļ�����Դ���ڼ���
Remarks:
Revision History:
2020-01-13   ��������    ����
*/
#pragma once
class Monitor
{
public:
	Monitor();
	~Monitor();
	float CalcHitRate(int count, int totalNum);
	unsigned int GetInfoSize();
private:
	float hitRate;
	unsigned int infoSize;
};

