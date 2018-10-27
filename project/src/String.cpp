#include "String.h"
#include <cstdarg>

bool String::Equals(const std::string &str1, const std::string &str2, bool caseSensitive)
{
	if (str1.size() != str2.size())
		return false;

	const auto size = str1.size() > str2.size() ? str2.size() : str1.size();
	for (size_t i = 0; i < size; i++)
		if (!caseSensitive && tolower(str1[i]) != tolower(str2[i]) || caseSensitive && str1[i] != str2[i])
			return false;

	return true;
}

bool String::Contains(const std::string &target, const std::string &find)
{
	return strstr(target.c_str(), find.c_str()) != nullptr;
}

std::string String::Format(const std::string &format, ...)
{
	va_list arguments;
	va_start(arguments, format);

	const auto length = _vscprintf(format.c_str(), arguments) + 1;

	std::string result;
	result.resize(length);

	vsprintf_s(const_cast<char *>(result.c_str()), length, format.c_str(), arguments);

	va_end(arguments);

	// Remove null terminator
	result.resize(length - 1);

	return result;
}

std::vector<std::string> String::Split(const std::string &str, const std::string &delimeter)
{
	auto input = str;
	std::vector<std::string> output;

	size_t i;
	while ((i = input.find(delimeter)) != std::string::npos)
	{
		output.push_back(input.substr(0, i));
		input.erase(0, i + delimeter.size());
	}

	output.push_back(input);

	return output;
}
