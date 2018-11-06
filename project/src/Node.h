#pragma once

#include "GraphicsManager.h"
#include "Memory.h"
#include "Utility/Exception.h"
#include <vector>

struct RenderContext
{
	GraphicsManager *Graphics;
};

DEFINE_EXCEPTION(NodeNotFoundException);

class Node
{
	std::string m_Name;
	Node *m_Parent;
	std::vector<Node *> m_Children;

public:
	Node(std::string name = "Node", Node *parent = nullptr);

	Node *GetParent() const;

	void AddChild(Node *node);
	void RemoveChild(Node *node);

	template<typename TNode = Node, typename... TArgs>
	TNode *CreateChild(TArgs... args)
	{
		auto node = New<TNode>(args...);
		node->m_Parent = this;
		m_Children.push_back(node);
		return node;
	}

	template<typename TNode = Node>
	TNode *GetChild(const std::string &name)
	{
		for (auto &node : m_Children)
		{
			if (node->m_Name == m_Name)
				return dynamic_cast<TNode>(node);
		}

		THROW_EXCEPTION(NodeNotFoundException, "Node does not exist: %s", name.c_str());
	}

	template<typename TNode = Node>
	std::vector<TNode *> GetChildren(const std::string &name)
	{
		std::vector<TNode *> result;
		for (auto &node : m_Children)
		{
			if (node->m_Name == m_Name)
				result.push_back(dynamic_cast<TNode *>(node));
		}

		return result;
	}

	template<typename TNode = Node>
	TNode *GetChild()
	{
		for (auto &node : m_Children)
		{
			auto res = dynamic_cast<TNode>(node);
			if (res)
				return res;
		}

		THROW_EXCEPTION(NodeNotFoundException, "Node does not exist: %s", typeid(TNode).name());
	}

	virtual ~Node();

	virtual void Compile();
	virtual void Render(RenderContext *context);
};