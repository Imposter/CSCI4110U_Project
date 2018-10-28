#pragma once

#include <string>
#include <vector>

class String
{
public:
	static bool Equals(const std::string &str1, const std::string &str2, bool caseSensitive = true);
	static bool Contains(const std::string &target, const std::string &find);
	static std::string Format(std::string format, ...);
	static std::vector<std::string> Split(const std::string &str, const std::string &delimeter);
	static std::string Join(const std::vector<std::string> &v, const std::string &delimiter);
};