#pragma once

#include "Object.h"
#include "RenderContext.h"
#include <string>
#include <glm/matrix.hpp>

// TODO: RenderContext
// TODO: CompileContext

// TODO: Base off node
class Renderable : public Object
{
	std::string m_Name;
	glm::mat4x4 m_Matrix;
	glm::mat4x4 m_MatrixWorldSpace;
	bool m_IsWSIdentity;

public:
	virtual ~Renderable() = default;
	// TODO: ...
	virtual void Render(RenderContext *context);
};