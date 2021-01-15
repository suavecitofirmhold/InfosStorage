#include "Config.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <windows.h>

Config::Config()
{
	LPTSTR lpPath = L".\\config.ini";
	//GetPrivateProfileString(L"Config", L"Refresh", L"", LiMingSex, 1, lpPath);
	m_refresh = GetPrivateProfileInt(L"config", L"Refresh", 10, lpPath);

	//GetPrivateProfileString("Fangfang", "Sex", "", FangfangSex, 6, lpPath);
	m_expire = GetPrivateProfileInt(L"config", L"Expire", 10, lpPath);
}


Config::~Config()
{
}

unsigned int Config::GetExpireTime()
{
	return m_expire;
}
unsigned int Config::GetRefreshTime()
{
	return m_refresh;
}