#include "Log.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::ofstream g_LogStream;
LogType g_LogType;
bool g_LogOpen;

void LogOpen(const std::string &fileName, LogType level)
{
	if (g_LogOpen)
		return;

	g_LogStream.open(fileName, std::ios::out | std::ios::ate);
	g_LogType = level;
	g_LogOpen = true;

	time_t currentTime;
	time(&currentTime);
	tm localTime;
	localtime_s(&localTime, &currentTime);

	g_LogStream << "==================================================" << std::endl;
	g_LogStream << "Log created on "
		<< std::setw(2) << std::setfill('0') << localTime.tm_mday << "/"
		<< std::setw(2) << std::setfill('0') << localTime.tm_mon + 1 << "/"
		<< std::setw(2) << std::setfill('0') << localTime.tm_year + 1900 << " "
		<< std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << localTime.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << localTime.tm_sec
		<< std::endl;
	g_LogStream << "==================================================" << std::endl;
	g_LogStream << std::endl;
}

void LogClose()
{
	if (!g_LogOpen)
		return;

	g_LogStream.close();
	g_LogOpen = false;
}

void LogWriteToStream(std::ostream &stream, const std::string &title, const std::string &message, const std::string &type, tm time)
{
	stream << "[" << std::setw(2) << std::setfill('0') << time.tm_hour
		<< ":" << std::setw(2) << std::setfill('0') << time.tm_min
		<< ":" << std::setw(2) << std::setfill('0') << time.tm_sec
		<< "][" << type << ":" << title.c_str()
		<< "]: " << message.c_str() << std::endl;
}

void LogWrite(LogType type, const std::string &title, std::string format, ...)
{
	va_list arguments;
	va_start(arguments, format);

	const auto length = _vscprintf(format.c_str(), arguments) + 1;

	std::string message;
	message.resize(length);
	fill(message.begin(), message.end(), 0);

	vsprintf_s(const_cast<char *>(message.data()), length, format.c_str(), arguments);

	va_end(arguments);

	const char *typeString;
	switch (type)
	{
	case kLogType_Error:
		typeString = "ERROR";
		break;
	case kLogType_Warn:
		typeString = "WARNING";
		break;
	case kLogType_Trace:
		typeString = "TRACE";
		break;
	case kLogType_Info:
		typeString = "INFO";
		break;
	default:
		typeString = "UNKN";
	}

	time_t currentTime;
	time(&currentTime);
	tm localTime{};
	localtime_s(&localTime, &currentTime);

	LogWriteToStream(std::cout, title, message, typeString, localTime);
	if (g_LogOpen) LogWriteToStream(g_LogStream, title, message, typeString, localTime);
}