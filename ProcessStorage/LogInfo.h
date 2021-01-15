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
#include <boost/log/trivial.hpp>

#define LOG_DEBUG\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::debug))
#define LOG_INFO\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::info))
#define LOG_ERROR\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::error))
#define LOG_WARNING\
    BOOST_LOG_SEV((MyLog::s_slg),(boost::log::trivial::warning))

class LogInfo
{
public:
	LogInfo();
	~LogInfo();

	static void Init(const std::string & dir);

	static void WriteLog(const std::string& msg);

	static boost::log::sources::severity_logger<boost::log::trivial::severity_level > s_slg;
};