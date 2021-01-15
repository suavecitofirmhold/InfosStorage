#include "Tool.h"
#include <iostream>
#include <windows.h>
using std::cout;
using std::endl;
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

int GetMd5(const std::wstring & path, std::string & md5)
{
	const int READ_MAX_LIMIT = 1024;
	auto h_file = _wfopen(path.c_str(), L"rb");
	if (h_file == NULL)
	{
		return -1;
	}
	char * buff = new char[READ_MAX_LIMIT];
	size_t len;

	unsigned char md5_dec[MD5_DIGEST_LENGTH] = { 0 };
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	while (0 != (len = fread(buff, 1, READ_MAX_LIMIT, h_file)))
	{
		MD5_Update(&md5_ctx, buff, len);
	}
	if (0 != ferror(h_file))
	{
		delete buff;
		fclose(h_file);
		return -1;
	}
	MD5_Final(md5_dec, &md5_ctx);
	delete buff;
	fclose(h_file);

	/*转成hex*/
	char hex[3] = { 0 };
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
	{
		sprintf(hex, "%02x", md5_dec[i]);
		md5 += hex;
	}
	return 0;
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

wstring string2wstring(const string& str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
									//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

string wstring2string(const wstring& wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}


//int main()
//{
//	cout << GetFileMd5("F:\\project\\vstest\\ProcessStorage\\Debug\\md5test.exe") << endl;
//	/*(void)GenerateStrBasedUuid(L"3211");
//	GenerateStrBasedUuid(L"3211");*/
//	getchar();
//	return 0;
//}
