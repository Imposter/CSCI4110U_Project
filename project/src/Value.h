#pragma once

#include "Memory.h"
#include "Utility/Exception.h"
#include <map>
#include <glm/glm.hpp>

DEFINE_EXCEPTION(KeyNotFoundException);
DEFINE_EXCEPTION(ValueTypeMismatchException);

enum ValueType
{
	kValueType_None,
	kValueType_Int32,
	kValueType_UInt32,
	kValueType_Float,
	kValueType_Double,
	kValueType_String,
	kValueType_Vec2,
	kValueType_Vec3,
	kValueType_Vec4,
	kValueType_Matrix4x4,
};

template<typename T, ValueType Type>
class Value
{
protected:
	ValueType m_Type;
	T *m_Value;

public:
	Value(T value)
		: m_Type(Type), m_Value(New<T>(value))
	{
	}

	~Value()
	{
		Delete(m_Value);
	}

	Value(const Value &copy)
		: m_Type(copy.m_Type), m_Value(New<T>(*copy.m_Value))
	{
	}

	Value &operator=(const Value &other)
	{
		m_Type = other.m_Type;

		// Delete current value and copy other
		Delete(m_Value);
		m_Value = New<T>(*other.m_Value);

		return *this;
	}

	// No moving
	Value(const Value &&) = delete;
	Value &operator=(const Value &&) = delete;

	ValueType GetType() const
	{
		return m_Type;
	}

	T &Get() const
	{
		return *m_Value;
	}

	void Set(T value)
	{
		*m_Value = value;
	}
};

typedef Value<int, kValueType_Int32> ValueInt32;
typedef Value<unsigned int, kValueType_UInt32> ValueUInt32;
typedef Value<float, kValueType_Float> ValueFloat;
typedef Value<double, kValueType_Double> ValueDouble;
typedef Value<std::string, kValueType_String> ValueString;
typedef Value<glm::vec2, kValueType_Vec2> ValueVec2;
typedef Value<glm::vec3, kValueType_Vec2> ValueVec3;
typedef Value<glm::vec4, kValueType_Vec2> ValueVec4;
typedef Value<glm::mat4x4, kValueType_Matrix4x4> ValueMatrix4x4;

class ValueContainer
{
	typedef Value<void *, kValueType_None> TValue;
	std::map<std::string, TValue *> m_Values;

public:
	template<typename T, ValueType Type>
	void Set(const std::string &key, const Value<T, Type> &value)
	{
		const auto v = reinterpret_cast<TValue *>(New<Value<T, Type>>(value));
		std::map<std::string, TValue *>::iterator it;
		if ((it = m_Values.find(key)) != m_Values.end())
			it->second = v;
		else m_Values.emplace(key, v);
	}

	template<typename T, ValueType Type>
	Value<T, Type> &Get(const std::string &key)
	{
		std::map<std::string, TValue *>::iterator it;
		if ((it = m_Values.find(key)) == m_Values.end())
			THROW_EXCEPTION(KeyNotFoundException, "Key %s not found", key.c_str());
		const auto v = reinterpret_cast<Value<T, Type> *>(it->second);
		const auto t = v->GetType();
		if (t != Type)
			THROW_EXCEPTION(ValueTypeMismatchException, "Value type mismatch (expected %d, got %d)", Type, t);
		return *v;
	}

	ValueContainer() = default;

	~ValueContainer()
	{
		for (auto &pair : m_Values)
			Delete(pair.second);
		m_Values.clear();
	}

	// No copying/moving
	ValueContainer(const ValueContainer &) = delete;
	ValueContainer &operator=(const ValueContainer &) = delete;

	ValueContainer(const ValueContainer &&) = delete;
	ValueContainer &operator=(const ValueContainer &&) = delete;

	void Get(const std::string &key, int &value)
	{
		value = Get<int, kValueType_Int32>(key).Get();
	}

	void Set(const std::string &key, int value)
	{
		Set<int, kValueType_Int32>(key, value);
	}

	void Get(const std::string &key, unsigned int &value)
	{
		value = Get<unsigned int, kValueType_UInt32>(key).Get();
	}

	void Set(const std::string &key, unsigned int value)
	{
		Set<unsigned int, kValueType_UInt32>(key, value);
	}

	void Get(const std::string &key, float &value)
	{
		value = Get<float, kValueType_Float>(key).Get();
	}

	void Set(const std::string &key, float value)
	{
		Set<float, kValueType_Float>(key, value);
	}

	void Get(const std::string &key, double &value)
	{
		value = Get<double, kValueType_Double>(key).Get();
	}

	void Set(const std::string &key, double value)
	{
		Set<double, kValueType_Double>(key, value);
	}

	void Get(const std::string &key, std::string &value)
	{
		value = Get<std::string, kValueType_String>(key).Get();
	}

	void Set(const std::string &key, const std::string &value)
	{
		Set<std::string, kValueType_String>(key, value);
	}

	void Get(const std::string &key, glm::vec2 &value)
	{
		value = Get<glm::vec2, kValueType_Vec2>(key).Get();
	}

	void Set(const std::string &key, const glm::vec2 &value)
	{
		Set<glm::vec2, kValueType_Vec2>(key, value);
	}

	void Get(const std::string &key, glm::vec3 &value)
	{
		value = Get<glm::vec3, kValueType_Vec3>(key).Get();
	}

	void Set(const std::string &key, const glm::vec3 &value)
	{
		Set<glm::vec3, kValueType_Vec3>(key, value);
	}

	void Get(const std::string &key, glm::vec4 &value)
	{
		value = Get<glm::vec4, kValueType_Vec4>(key).Get();
	}

	void Set(const std::string &key, const glm::vec4 &value)
	{
		Set<glm::vec4, kValueType_Vec4>(key, value);
	}

	void Get(const std::string &key, glm::mat4x4 &value)
	{
		value = Get<glm::mat4x4, kValueType_Matrix4x4>(key).Get();
	}

	void Set(const std::string &key, const glm::mat4x4 &value)
	{
		Set<glm::mat4x4, kValueType_Matrix4x4>(key, value);
	}
};