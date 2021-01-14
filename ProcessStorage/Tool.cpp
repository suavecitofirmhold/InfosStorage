#include "Tool.h"
#include <iostream>
#include <windows.h>
using std::cout;
using std::endl;
using std::wstring;

std::string GetFileMd5(const std::string& path)
{
	using namespace std;
	int len = 0;
	unsigned char buffer[1024] = { 0 };
	unsigned char digest[16] = { 0 };
	FILE *pFile = fopen(path.c_str(), "rb");
	if (nullptr == pFile)
	{
		cout << "open err" << endl;
		return "";
	}
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	while ((len = fread(buffer, 1, 1024, pFile)) > 0)
	{
		MD5_Update(&md5_ctx, buffer, len);
	}
	MD5_Final(digest, &md5_ctx);
	fclose(pFile);

	int i = 0;
	char buf[33] = { 0 };
	char tmp[3] = { 0 };
	for (i = 0; i < 16; i++)
	{
		sprintf(tmp, "%02X", digest[i]);
		strcat(buf, tmp);
	}
	//cout << string(buf) << endl;
	return string(buf);
}


boost::uuids::uuid GenerateStrBasedUuid(const std::wstring& seed)
{
	using namespace boost::uuids;
	uuid stringBaseUuid = string_generator()("{0123456789abcdef0123456789abcdef}");
	name_generator ngen(stringBaseUuid);
	uuid u1 = ngen(seed);
	//cout << u1 << endl;
	return u1;
}


//int main()
//{
//	cout << GetFileMd5("F:\\project\\vstest\\ProcessStorage\\Debug\\md5test.exe") << endl;
//	/*(void)GenerateStrBasedUuid(L"3211");
//	GenerateStrBasedUuid(L"3211");*/
//	getchar();
//	return 0;
//}
