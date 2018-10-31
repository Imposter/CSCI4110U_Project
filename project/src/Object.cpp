#include "Object.h"

Object::Object(std::string name, Object *parent)
	: m_Name(std::move(name)), m_IsActive(true), m_Parent(parent), m_Transform(parent ? &parent->m_Transform : nullptr)
{
}

const std::string &Object::GetName() const
{
	return m_Name;
}

const bool &Object::IsActive() const
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
	obj->m_Transform.SetParent(&m_Transform);
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
	obj->m_Transform.SetParent(nullptr);
}

Transform *Object::GetTransform()
{
	return &m_Transform;
}

Object::~Object()
{
	if (m_Parent)
		m_Parent->RemoveChild(this);

	for (auto &obj : m_Children)
		Delete(obj);

	m_Children.clear();
}

void Object::Update(float deltaTime)
{
	// Update all children
	for (auto &obj : m_Children)
		obj->Update(deltaTime);
}

void Object::Render(float deltaTime)
{
	// Render all children
	for (auto &obj : m_Children)
		obj->Render(deltaTime);
}

void Object::Shutdown()
{
	// Shutdown all children
	for (auto &obj : m_Children)
		obj->Shutdown();
}