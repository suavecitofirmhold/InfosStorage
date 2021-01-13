#include "InfoHandle.hpp"
#include <openssl/md5.h>
//#include <objbase.h>
#include <sstream>

InfoHandle::InfoHandle()
{
}


InfoHandle::~InfoHandle()
{
}

void InfoHandle::SetExitProcess(std::string& guid)
{
	ProcessInfoCache::GetInstance()->ModifyExitFlag(guid, true);
}

void InfoHandle::ClearExpiredInfo()
{
	ProcessInfoCache::GetInstance()->EraseExpiredData();
}

std::string InfoHandle::Change2Md5(processInfo& p)
{
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	wchar_t temp[64];
	std::wstring tempArr = p.fileName + p.szExeFile + p.userName + std::wstring(_itow(p.pid, temp, 10));
	const int md5Length = 16;
	unsigned char md5Result[md5Length];
	MD5((const unsigned char*)tempArr.c_str(), tempArr.size() * 2, md5Result);
	return std::string((char*)md5Result);
}
void InfoHandle::GetCurrent(SYSTEMTIME& st)
{
	GetSystemTime(&st);
}

DWORD InfoHandle::ChangeSysTime2NumType(SYSTEMTIME& st)
{
	DWORD numType =
		st.wHour * 3600000   // 60 * 60 * 1000
		+ st.wMinute * 6000  // 60 * 1000
		+ st.wSecond * 1000
		+ st.wMilliseconds;
	return numType;
}

void InfoHandle::RemoveDuplication()
{

}

std::string InfoHandle::DataSupplement()
{
	using std::stringstream;
	GUID guid;
	CoCreateGuid(&guid);
	stringstream sstream;
	sstream << guid.Data1 << guid.Data2 << guid.Data3;
	for (int i = 0; i < 8; ++i)
	{
		sstream << guid.Data4[i];
	}
	return sstream.str();
}