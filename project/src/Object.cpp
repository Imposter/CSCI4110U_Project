#include "Object.h"

Object::Object(std::string name, Object *parent)
	: m_Name(std::move(name)), m_IsActive(true), m_Parent(parent)
{
}

const std::string &Object::GetName() const
{
	return m_Name;
}

bool Object::IsActive() const
{
	return m_IsActive;
}

void Object::SetActive(bool active)
{
	m_IsActive = active;
}

Object *Object::GetParent() const
{
	return m_Parent;
}

void Object::AddChild(Object *obj)
{
	if (obj->m_Parent)
		obj->m_Parent->RemoveChild(obj);

	obj->m_Parent = this;
	m_Children.push_back(obj);
}

void Object::RemoveChild(Object *obj)
{
	if (obj->m_Parent != this)
		return;

	for (auto it = m_Children.begin(); it != m_Children.end();)
	{
		if (*it == obj)
		{
			m_Children.erase(it);
			break;
		}
		++it;
	}

	obj->m_Parent = nullptr;
}

Object::~Object()
{
	for (auto obj : m_Children)
		Delete(obj);

	m_Children.clear();
}

void Object::Update(float time, float deltaTime)
{
	// Update all children
	for (auto obj : m_Children)
		if (obj->m_IsActive)
			obj->Update(time, deltaTime);
}

void Object::Render(float time, float deltaTime)
{
	// Render all children
	for (auto obj : m_Children)
		if (obj->m_IsActive)
			obj->Render(time, deltaTime);
}

void Object::Shutdown()
{
	// Shutdown all children
	for (auto obj : m_Children)
		obj->Shutdown();
}