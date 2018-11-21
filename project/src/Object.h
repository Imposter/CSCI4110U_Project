#pragma once

#include "Transform.h"
#include "Memory.h"
#include "Utility/Exception.h"
#include <string>
#include <vector>
#include <memory>

DEFINE_EXCEPTION(ObjectNotFoundException);

class Object
{
protected:
	std::string m_Name;
	bool m_IsActive;
	Object *m_Parent;
	std::vector<Object *> m_Children;

public:
	Object(std::string name = "Object", Object *parent = nullptr);

	// No copying/moving
	Object(const Object &) = delete;
	Object &operator=(const Object &) = delete;

	Object(const Object &&) = delete;
	Object &operator=(const Object &&) = delete;

	const std::string &GetName() const;

	bool IsActive() const;
	void SetActive(bool active);

	Object *GetParent() const;

	void AddChild(Object *obj);
	void RemoveChild(Object *obj);

	template<typename TObj = Object, typename... TArgs>
	TObj *CreateChild(TArgs... args)
	{
		auto obj = New<TObj>(args...);
		obj->m_Parent = this;
		m_Children.push_back(obj);
		return obj;
	}
	
	template<typename TObj = Object>
	TObj *GetChild(const std::string &name)
	{
		for (auto &obj : m_Children)
		{
			if (obj->m_Name == m_Name)
				return dynamic_cast<TObj>(obj);
		}

		THROW_EXCEPTION(ObjectNotFoundException, "Object does not exist: %s", name.c_str());
	}

	template<typename TObj = Object>
	std::vector<TObj *> GetChildren(const std::string &name)
	{
		std::vector<TObj *> result;
		for (auto &node : m_Children)
		{
			if (node->m_Name == m_Name)
				result.push_back(dynamic_cast<TObj *>(node));
		}

		return result;
	}

	template<typename TObj = Object>
	TObj *GetChild()
	{
		for (auto &obj : m_Children)
		{
			auto res = dynamic_cast<TObj>(obj);
			if (res)
				return res;
		}

		THROW_EXCEPTION(ObjectNotFoundException, "Object does not exist: %s", typeid(TObj).name());
	}

	virtual ~Object();
	
	virtual void Update(float time, float deltaTime);	
	virtual void Render(float time, float deltaTime);	
	virtual void Shutdown();
};
