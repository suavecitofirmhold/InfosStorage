#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/md5.h>

using std::wstring;
using std::string;
std::string GetFileMd5(const std::string& path);
int GetMd5(const std::wstring & path, std::string & md5);
boost::uuids::uuid GenerateStrBasedUuid(const std::wstring& seed);
wstring string2wstring(const string& str);
string wstring2string(const wstring& wstr);