#include "Exception.h"
#include <utility>

Exception::Exception(int code, std::string type, std::string message)
	: runtime_error(""), m_Code(code), m_Type(std::move(type)), m_Message(std::move(message))
{
	if (String::Equals(m_Type, ""))
		m_What = String::Format("%d - %s", m_Code, m_Message.c_str());
	else m_What = String::Format("[%s] %d - %s", m_Type.c_str(), m_Code, m_Message.c_str());
}

Exception::Exception(std::string type, std::string message)
	: runtime_error(""), m_Code(-1), m_Type(std::move(type)), m_Message(std::move(message))
{
	if (String::Equals(m_Type, ""))
		m_What = m_Message;
	else m_What = String::Format("[%s] %s", m_Type.c_str(), m_Message.c_str());
}

Exception::Exception(std::string message)
	: runtime_error(""), m_Code(-1), m_Type(""), m_Message(std::move(message))
{
	m_What = m_Message;
}

const char *Exception::what() const
{
	return m_What.c_str();
}
