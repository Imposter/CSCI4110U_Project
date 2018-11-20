#include "Node.h"

Node::Node(std::string name, Node *parent)
	: m_Name(name), m_Parent(parent)
{
}

const std::string &Node::GetName() const
{
	return m_Name;
}

Node *Node::GetParent() const
{
	return m_Parent;
}

void Node::AddChild(Node *node)
{
	if (node->m_Parent)
		node->m_Parent->RemoveChild(node);

	node->m_Parent = this;
	m_Children.push_back(node);
}

void Node::RemoveChild(Node *node)
{
	if (node->m_Parent != this)
		return;

	for (auto it = m_Children.begin(); it != m_Children.end();)
	{
		if (*it == node)
		{
			m_Children.erase(it);
			break;
		}
		++it;
	}

	node->m_Parent = nullptr;
}

Node::~Node()
{
	if (m_Parent)
		m_Parent->RemoveChild(this);

	for (auto &obj : m_Children)
		Delete(obj);

	m_Children.clear();
}

void Node::Compile()
{
	for (auto &node : m_Children)
		node->Compile();
}

void Node::Render(RenderContext *context)
{
	for (auto &node : m_Children)
		node->Render(context);
}