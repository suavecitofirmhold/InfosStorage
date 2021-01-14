#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/md5.h>

std::string GetFileMd5(const std::string& path);
boost::uuids::uuid GenerateStrBasedUuid(const std::wstring& seed);