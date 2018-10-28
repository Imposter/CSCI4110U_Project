#include "Exception.h"
#include <utility>

Exception::Exception(int code, std::string type, std::string message)
	: runtime_error(""), m_Code(code), m_Type(std::move(type)), m_Message(std::move(message)) {}

Exception::Exception(std::string type, std::string message)
	: runtime_error(""), m_Code(-1), m_Type(std::move(type)), m_Message(std::move(message)) {}

Exception::Exception(std::string message)
	: runtime_error(""), m_Code(-1), m_Type(""), m_Message(std::move(message)) {}

const char *Exception::what() const
{
	if (m_Code < 0 && String::Equals(m_Type, ""))
		return m_Message.c_str();
	if (m_Code < 0)
		return String::Format("[%s] %s", m_Type.c_str(), m_Message.c_str()).c_str();
	if (String::Equals(m_Type, ""))
		return String::Format("%d - %s", m_Code, m_Message.c_str()).c_str();
	return String::Format("[%s] %d - %s", m_Type.c_str(), m_Code, m_Message.c_str()).c_str();
}
