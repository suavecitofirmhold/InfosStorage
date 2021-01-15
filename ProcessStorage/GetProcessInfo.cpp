#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <cstdio>
#include <psapi.h>
#include <set>
#include "LogInfo.h"
#include "GetProcessInfo.h"
#include "ProcessInfoCache.h"
#include "Tool.h"
using namespace std;

void printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);

}

wstring GetProcessInfo::DosDevicePath2LogicalPath(LPCTSTR lpszDosPath)
{
	wstring strResult;

	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';

	if (lpszDosPath == NULL || !GetLogicalDriveStrings(_countof(szTemp) - 1, szTemp)) {
		return strResult;
	}
	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;
	do {
		// Copy the drive letter to the template string
		*szDrive = *p;

		// Look up each device name
		if (QueryDosDevice(szDrive, szName, _countof(szName))) {
			UINT uNameLen = (UINT)_tcslen(szName);

			if (uNameLen < MAX_PATH)
			{
				bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

				if (bFound) {
					// Reconstruct pszFilename using szTemp
					// Replace device path with DOS path
					TCHAR szTempFile[MAX_PATH];
					_stprintf_s(szTempFile, TEXT("%s%s"), szDrive, lpszDosPath + uNameLen);
					strResult = wstring(szTempFile);
					//wcout << "pathpathpath   :  " << szTempFile << endl;
					wcout << "wstring path   : " << strResult << endl;
				}
			}
		}
		// Go to the next NULL character.
		while (*p++);
	} while (!bFound && *p); // end of string

	return strResult;
}
LPWSTR GetProcessInfo::GetProcessUserName(DWORD pid)
{
	HANDLE hToken = NULL;
	BOOL bFuncReturn = FALSE;
	LPWSTR strUserName = _T("");
	PTOKEN_USER pToken_User = NULL;
	DWORD dwTokenUser = 0;
	TCHAR szAccName[MAX_PATH] = { 0 };
	TCHAR szDomainName[MAX_PATH] = { 0 };
	HANDLE hProcessToken = NULL;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (hProcess != NULL)
	{
		// 提升本进程的权限
		bFuncReturn = ::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);

		if (bFuncReturn == 0) // 失败
		{
			printError(TEXT("OpenProcessToken err"));
			return strUserName;
		}
		if (hToken != NULL)
		{
			::GetTokenInformation(hToken, TokenUser, NULL, 0L, &dwTokenUser);

			if (dwTokenUser>0)
			{
				pToken_User = (PTOKEN_USER)::GlobalAlloc(GPTR, dwTokenUser);
			}

			if (pToken_User != NULL)
			{
				bFuncReturn = ::GetTokenInformation(hToken, TokenUser, pToken_User, dwTokenUser, &dwTokenUser);
			}

			if (bFuncReturn != FALSE && pToken_User != NULL)
			{
				SID_NAME_USE eUse = SidTypeUnknown;

				DWORD dwAccName = 0L;
				DWORD dwDomainName = 0L;

				PSID pSid = pToken_User->User.Sid;

				bFuncReturn = ::LookupAccountSid(NULL, pSid, NULL, &dwAccName,
					NULL, &dwDomainName, &eUse);
				if (dwAccName>0 && dwAccName < MAX_PATH && dwDomainName>0 && dwDomainName <= MAX_PATH)
				{
					bFuncReturn = ::LookupAccountSid(NULL, pSid, szAccName, &dwAccName,
						szDomainName, &dwDomainName, &eUse);
				}

				if (bFuncReturn != 0)
					strUserName = szAccName;
			}
		}
	}

	if (pToken_User != NULL)
	{
		::GlobalFree(pToken_User);
	}

	if (hToken != NULL)
	{
		::CloseHandle(hToken);
	}

	return strUserName;
}

BOOL GetProcessInfo::GetProcessList()
{
	m_log.WriteLog("in GetProcessList()");
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	_tprintf(TEXT("\nPROCESSENTRY32 SIZE0:  %d"), pe32.dwSize);
	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}
	std::set<boost::uuids::uuid> uuidSet;
	
	do
	{
		std::shared_ptr<processInfo> sp(new processInfo());

		_tprintf(TEXT("\n\n====================================================="));
		_tprintf(TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile);
		_tprintf(TEXT("\n-------------------------------------------------------"));
		sp->fileName = wstring(pe32.szExeFile);
		// Retrieve the priority class.
		dwPriorityClass = 0;
		FILETIME loadStartTime{0, 0};
		FILETIME exitTime{0, 0};
		FILETIME kernelTime{0, 0};
		FILETIME userTime{0, 0};
		BOOL getTimeOk = FALSE;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			printError(TEXT("OpenProcess"));
		else
		{
			_tprintf(TEXT("\n  Process ID        = 0x%08X"), pe32.th32ProcessID);
			_tprintf(TEXT("\n  Parent process ID = 0x%08X"), pe32.th32ParentProcessID);
			sp->pid = pe32.th32ProcessID;
			sp->ppid = pe32.th32ParentProcessID;
			getTimeOk = GetProcessTimes(hProcess, &loadStartTime, &exitTime, &kernelTime, &userTime);
			if (0 != getTimeOk) {
				SYSTEMTIME beginTime;
				::FileTimeToSystemTime(&loadStartTime, &beginTime);
				DWORD dwBeginTime = dp.ChangeSysTime2NumType(beginTime);
				_tprintf(TEXT("\n  Hour = %d Minute = %d Second = %d Millisec = %d"), beginTime.wHour, beginTime.wMinute, beginTime.wSecond, beginTime.wMilliseconds);
				_tprintf(TEXT("\n  Begin Time  = %d"), dwBeginTime);
				sp->startTime = dwBeginTime;
				// todo : 进程命令行信息获取，

			}
			// 获取进程路径
			TCHAR tsFileDosPath[MAX_PATH + 1];
			if (GetProcessImageFileName(hProcess, tsFileDosPath, 128) > 0)
			{
				wstring logicFilePath = DosDevicePath2LogicalPath(tsFileDosPath);
				_tprintf(TEXT("\n     tsFileDosPath   = %s"), logicFilePath);
				sp->filePath = logicFilePath;
				// todo: 获取md5
				//pInfo.mdFive = GetFileMd5();  uuid用DataSupplement
				string md5 = "";
				if (0 == GetMd5(sp->filePath, md5))
				{

				}
			}
			else {
				printError(TEXT("get path failed"));
			}
			boost::uuids::uuid ud = dp.DataSupplement(sp);  // 根据关键字获取uuid
			if (!dp.IsDuplication(ud)) {  // 判断是否重复，不重复则添加
				dp.AddNew(ud, sp);
			}
			CloseHandle(hProcess);
			uuidSet.insert(ud);
		}
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	dp.SetExiteTime(uuidSet);
	dp.ClearExpiredInfo(m_expireTime);
	return(TRUE);
}

void GetProcessInfo::Init(LogInfo& log, const unsigned int exp)
{
	m_log = log;
	m_expireTime = exp;
}

//int main(void)
//{
//	//GetProcessList();
//	LogInfo log;
//	log.Init("F:\\Debug_x64");// 日志文件存放的路径
//	log.WriteLog("start");
//	GetProcessInfo gp;
//	gp.GetProcessList();
//	getchar();
//	return 0;
//}