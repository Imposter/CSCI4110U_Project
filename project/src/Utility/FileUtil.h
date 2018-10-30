#pragma once

#include "Exception.h"
#include <vector>
#include <string>

DEFINE_EXCEPTION(FileNotFoundException);

class File
{
public:
	static bool Exists(const std::string &path);
	static std::vector<std::string> ReadAllLines(const std::string &path);
};