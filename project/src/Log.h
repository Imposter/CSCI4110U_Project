#pragma once

#include <string>

enum LogType
{
	kLogType_Info,
	kLogType_Error,
	kLogType_Warn,
	kLogType_Trace
};

void LogOpen(const std::string &fileName, LogType level);
void LogClose();
void LogWrite(LogType type, const std::string &title, const std::string &format, ...);

#define LOG_INFO(title, format, ...) LogWrite(kLogType_Info, title, format, __VA_ARGS__)
#define LOG_ERROR(title, format, ...) LogWrite(kLogType_Error, title, format, __VA_ARGS__)
#define LOG_WARN(title, format, ...) LogWrite(kLogType_Warn, title, format, __VA_ARGS__)

#ifdef DEBUG
#define LOG_TRACE(title, format, ...) LogWrite(kLogType_Trace, title, format, __VA_ARGS__)
#else
#define LOG_TRACE(title, format, ...)
#endif