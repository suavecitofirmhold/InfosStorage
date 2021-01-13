#include <cstdio>
#include <string>
#include <iostream>
#include <openssl/md5.h>
#include <sstream>
using namespace std;

int main()
{
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	std::wstring tempArr = L"°¡¹ş¹ş¹şÎûÎûÎûÎû";
	const int md5Length = 16;
	unsigned char md5Result[md5Length];
	MD5((const unsigned char*)tempArr.c_str(), tempArr.size() * 2, md5Result);
	for (int i = 0; i < md5Length; i++) {
		cout << hex << (int)md5Result[i];
	}
	cout << endl;
	std::string ss((const char*)md5Result);
	cout << ss << endl;
	wchar_t str[64];
	wcout << wstring(_itow(12345, str, 10)) << endl;
	wcout << str << endl;

	unsigned int abcd[] = { 123, 987654, 000000000000000 };
	ostringstream oss;
	oss << abcd;
	cout << oss.str() << endl;
	getchar();
	return 0;
}