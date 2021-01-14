#include <cstdio>
#include <string>
#include <iostream>
#include <openssl/md5.h>
#include <sstream>
#include <windows.h>
using namespace std;
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace boost::uuids;
//int main()
//{
//	/*MD5_CTX md5_ctx;
//	MD5_Init(&md5_ctx);
//	std::wstring tempArr = L"����������������";
//	const int md5Length = 16;
//	unsigned char md5Result[md5Length];
//	MD5((const unsigned char*)tempArr.c_str(), tempArr.size() * 2, md5Result);
//	for (int i = 0; i < md5Length; i++) {
//		cout << hex << (int)md5Result[i];
//	}
//	cout << endl;
//	std::string ss((const char*)md5Result);
//	cout << ss << endl;
//	wchar_t str[64];
//	wcout << wstring(_itow(12345, str, 10)) << endl;
//	wcout << str << endl;
//	ss.clear();
//
//	uuid www_xxx_com = string_generator()
//		("{0123456789abcdef0123456789abcdef}");
//	name_generator ngen(www_xxx_com);
//
//	uuid u1 = ngen("mario");
//	assert(!u1.is_nil() &&
//		u1.version() == uuid::version_name_based_sha1);
//	wstring w1 = L"aŶ��";
//	DWORD d1 = 123;
//	wchar_t temp[64];
//	wstring ws = w1 + std::wstring(_itow(d1, temp, 10));
//	uuid u2 = ngen(ws);
//	cout << "uuid2 "<< 
//		u2 << endl;
//	uuid u3 = ngen(ws);
//	cout << "uuid3 " <<
//		u2 << endl;*/
//
//	getchar();
//	return 0;
//}

// one can also use Winternl.h if needed 
//#include <Winternl.h> // for UNICODE_STRING and SYSTEM_INFORMATION_CLASS 
#include <stdio.h>
#include <tchar.h> 
#define STATUS_SUCCESS    ((NTSTATUS)0x00000000L) 
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L) 

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemProcessInformation = 5
} SYSTEM_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING;

typedef LONG KPRIORITY; // Thread priority 

typedef struct _SYSTEM_PROCESS_INFORMATION_DETAILD {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
	ULONG HandleCount;
	BYTE Reserved4[4];
	PVOID Reserved5[11];
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER Reserved6[6];
} SYSTEM_PROCESS_INFORMATION_DETAILD, *PSYSTEM_PROCESS_INFORMATION_DETAILD;

typedef NTSTATUS(WINAPI *PFN_NT_QUERY_SYSTEM_INFORMATION)(
	IN  SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN  ULONG SystemInformationLength,
	OUT OPTIONAL PULONG ReturnLength
	);

int main()
{
	size_t bufferSize = 102400;
	PSYSTEM_PROCESS_INFORMATION_DETAILD pspid =
		(PSYSTEM_PROCESS_INFORMATION_DETAILD)malloc(bufferSize);
	ULONG ReturnLength;
	PFN_NT_QUERY_SYSTEM_INFORMATION pfnNtQuerySystemInformation = (PFN_NT_QUERY_SYSTEM_INFORMATION)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtQuerySystemInformation");
	NTSTATUS status;

	while (TRUE) {
		status = pfnNtQuerySystemInformation(SystemProcessInformation, (PVOID)pspid,
			bufferSize, &ReturnLength);
		if (status == STATUS_SUCCESS)
			break;
		else if (status != STATUS_INFO_LENGTH_MISMATCH) { // 0xC0000004L 
			_tprintf(TEXT("ERROR 0x%X\n"), status);
			return 1; // error 
		}

		bufferSize *= 2;
		pspid = (PSYSTEM_PROCESS_INFORMATION_DETAILD)realloc((PVOID)pspid, bufferSize);
	}

	for (;;
		pspid = (PSYSTEM_PROCESS_INFORMATION_DETAILD)(pspid->NextEntryOffset + (PBYTE)pspid)) {

		_tprintf(TEXT("ProcessId: %d, ImageFileName: %ls\n"), pspid->UniqueProcessId,
			(pspid->ImageName.Length && pspid->ImageName.Buffer) ? pspid->ImageName.Buffer : L"");

		if (pspid->NextEntryOffset == 0) break;
	}
	getchar();
	return 0;
}