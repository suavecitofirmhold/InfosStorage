#include <cstdio>
#include <Windows.h>
#include <tchar.h>
#include <thread>
#include "LogInfo.h"
#include "GetProcessInfo.h"
#include "Service.h"
#include "Config.h"
#include "Monitor.h"

SERVICE_STATUS_HANDLE ssh = NULL; // 全局句柄，保存服务控制请求的句柄
SERVICE_STATUS ss = { 0 }; //保存服务信息的结构
void PrintError(wchar_t *err) //打印错误信息到控制台
{
	printf("%s ErrorCode : %d\r\n", err, GetLastError());
}

void Init()
{
	m_log.Init("F:\\Debug_x64");// 日志文件存放的路径
	m_log.WriteLog("start");
	Config cfg;
	expireTime = cfg.GetExpireTime();
	refreshTime = cfg.GetRefreshTime();
	getInfo.Init(m_log, expireTime);
}
BOOL InstallService() //安装服务
{
	wchar_t DirBuf[1024] = { 0 }, SysDir[1024] = { 0 };
	GetCurrentDirectory(1024, DirBuf);
	GetModuleFileName(NULL, DirBuf, sizeof(DirBuf));
	GetSystemDirectory(SysDir, sizeof(SysDir));
	wcscat_s(SysDir, L"\\ExampleService.exe");
	if (!CopyFile(DirBuf, SysDir, FALSE))
	{
		m_log.WriteLog("CopyFile Fail");
		PrintError(L"CopyFile Fail");
		return FALSE;
	}

	SC_HANDLE sch = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!sch)
	{
		m_log.WriteLog("OpenSCManager Failed");
		PrintError(L"OpenSCManager Failed");
		return FALSE;
	}

	SC_HANDLE schNewSrv = CreateService(sch, L"ExampleService", L"InfoServiceApp", SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL, SysDir, NULL, NULL, NULL, NULL, NULL);

	if (!schNewSrv)
	{
		m_log.WriteLog("CreateService Failed");
		PrintError(L"CreateService Failed");
		return FALSE;
	}

	SERVICE_DESCRIPTION sd;
	sd.lpDescription = L"A Sample Service , Test Example";

	ChangeServiceConfig2(schNewSrv, SERVICE_CONFIG_DESCRIPTION, &sd);
	CloseServiceHandle(schNewSrv);
	CloseServiceHandle(sch);
	m_log.WriteLog("Install Service Success!");
	printf("Install Service Success!");
	return TRUE;
}

BOOL UnInstallService() //卸载服务
{
	SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scm)
	{
		PrintError(L"OpenSCManager Failed");
		return FALSE;
	}

	SC_HANDLE scml = OpenService(scm, L"ExampleService", SC_MANAGER_ALL_ACCESS);
	if (!scml)
	{
		PrintError(L"OpenService Failed");
		return FALSE;
	}
	SERVICE_STATUS ss;
	if (!QueryServiceStatus(scml, &ss))
	{
		PrintError(L"QueryServiceStatus Failed");
		return FALSE;
	}
	if (ss.dwCurrentState != SERVICE_STOPPED)
	{
		if (!ControlService(scml, SERVICE_CONTROL_STOP, &ss) && ss.dwCurrentState != SERVICE_CONTROL_STOP)
		{
			PrintError(L"ControlService Stop Failed");
			return FALSE;
		}
	}
	if (!DeleteService(scml))
	{
		PrintError(L"DeleteService Failed");
		return FALSE;
	}
	printf("Delete Service Success!");
	return TRUE;
}

void WINAPI ServiceCtrlHandler(DWORD dwOpcode) //服务控制函数
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		ss.dwCurrentState = SERVICE_STOPPED;
		break;
	case SERVICE_CONTROL_PAUSE:
		ss.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		ss.dwCurrentState = SERVICE_CONTINUE_PENDING;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		PrintError(L"bad service request");
	}
	SetServiceStatus(ssh, &ss);
}
//服务入口函数
VOID WINAPI ServiceMain(
	DWORD dwArgc,     // number of arguments
	LPTSTR *lpszArgv  // array of arguments
)
{
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = SERVICE_START_PENDING;
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ss.dwCheckPoint = 0;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwWaitHint = 0;
	ss.dwWin32ExitCode = 0;

	ssh = RegisterServiceCtrlHandler(L"ExampleService", ServiceCtrlHandler);

	if (!ssh)
	{
		PrintError(L"RegisterService Fail");
		return;
	}
	if (!SetServiceStatus(ssh, &ss))
	{
		PrintError(L"SetServiceStatus 0x01 Fail");
		return;
	}

	ss.dwWin32ExitCode = S_OK;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	ss.dwCurrentState = SERVICE_RUNNING;
	if (!SetServiceStatus(ssh, &ss))
	{
		PrintError(L"SetServiceStatus 0x02 Fail");
		return;
	}

	Init();
	while (1)
	{
		getInfo.GetProcessList();
		//do something， 在此处执行程序主逻辑
		Sleep(refreshTime * 6000);
	}

}

void usage() //打印帮助信息
{
	printf("[[-i Install],[-r UnInstall]]");
}

int wmain(int argc, _TCHAR* argv[]) //入口函数
{
	if (argc == 2)
	{
		//if arguments has 2
		wchar_t buf[10] = { 0 };
		wcscpy_s(buf, argv[1]);
		if (0 == wcscmp(buf, L"-i"))
		{
			if (!InstallService())
			{
				PrintError(L"Install Service Failed");
				return -1;
			}
		}
		else if (0 == wcscmp(buf, L"-r"))
		{
			if (!UnInstallService())
				return -1;
			else
				return 0;
		}
	}
	else if (argc > 2)
	{
		usage();
		return -1;
	}


	SERVICE_TABLE_ENTRY srvEntry[] = {
		{ L"ExampleService",ServiceMain },
		{ NULL,NULL }
	};
	StartServiceCtrlDispatcher(srvEntry);
	return 0;
}
