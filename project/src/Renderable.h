#pragma once

#include "RenderContext.h"
#include <string>
#include <glm/matrix.hpp>

// TODO: RenderContext
// TODO: CompileContext

// TODO: Base off node
class Renderable
{
	std::string m_Name;
	glm::mat4x4 m_Matrix;
	glm::mat4x4 m_MatrixWorldSpace;

public:
	virtual ~Renderable() = default;
	// TODO: ...
	virtual void Render(RenderContext *context);
};