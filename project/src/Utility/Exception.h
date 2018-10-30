#pragma once

#include "StringUtil.h"
#include <stdexcept>

class Exception : public std::runtime_error
{
	int m_Code;
	std::string m_Type;
	std::string m_Message;

	std::string m_What;

public:
	Exception(int code, std::string type, std::string message);
	Exception(std::string type, std::string message);
	Exception(std::string message);

	const char *what() const override;
};

#define __EXCEPTION_STRINGIFY(str) #str

#define DEFINE_EXCEPTION(name) class name : public Exception \
	{ \
	public: \
	explicit name(const std::string &message) \
	: Exception(__EXCEPTION_STRINGIFY(name), message) {} \
	}

#define THROW_EXCEPTION(name, format, ...) throw name(String::Format(format, __VA_ARGS__))