#pragma once

#include "Game.h"
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
#include <memory>

class GameObject
{
	Game *m_Game;
	std::string m_Name;
	bool m_IsActive;
	glm::mat4x4 m_Transform;
	GameObject *m_Parent;
	std::vector<std::shared_ptr<GameObject>> m_Children;

public:
	GameObject(Game *game, std::string name)
	{
		m_Game = game;
		m_Name = name;
		m_IsActive = true;
		m_Parent = nullptr;
	}

	const std::string &GetName() const
	{
		return m_Name;
	}

	const bool &IsActive() const
	{
		return m_IsActive;
	}

	const glm::mat4x4 &GetTransform() const
	{
		return m_Transform;
	}

	void SetTransform(const glm::mat4x4 &transform)
	{
		m_Transform = transform;
	}

	GameObject *GetParent() const
	{
		return m_Parent;
	}

	template<typename TObj = GameObject, typename... TArgs>
	std::shared_ptr<TObj> CreateChild(TArgs... args)
	{
		auto go = std::make_shared<TObj>(std::forward<TArgs>(args)...);
		go->m_Parent = this;
		m_Children.push_back(go);
		return go;
	}

	void AddChild(std::shared_ptr<GameObject> go)
	{
		go->m_Parent = this;
		m_Children.push_back(go);
	}
	
	template<typename TObj = GameObject>
	std::shared_ptr<TObj> GetChild(const std::string &name)
	{
		for (auto &go : m_Children)
		{
			if (go->m_Name == m_Name)
				return std::dynamic_pointer_cast<TObj>(go);
		}

		// TODO: Raise exception
		return nullptr;
	}

	template<typename TObj = GameObject>
	std::shared_ptr<TObj> GetChild()
	{
		for (auto &go : m_Children)
		{
			auto res = std::dynamic_pointer_cast<TObj>(go);
			if (res)
				return res;
		}

		// TODO: Raise exception -- error class
		return nullptr;
	}

	virtual ~GameObject() = default;
	
	virtual void Update(float deltaTime)
	{
		// Update all children
		for (auto &go : m_Children)
			go->Update(deltaTime);
	}
	
	virtual void Render(float deltaTime)
	{
		// Render all children
		for (auto &go : m_Children)
			go->Render(deltaTime);
	}
	
	virtual void RenderAudio(float deltaTime)
	{
		// Render all children
		for (auto &go : m_Children)
			go->RenderAudio(deltaTime);
	}
	
	virtual void Shutdown()
	{
		// Shutdown all children
		for (auto &go : m_Children)
			go->Shutdown();
	}
};
