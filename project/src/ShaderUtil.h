#pragma once

#include "Utility/Exception.h"
#include "Shader.h"

// Exception definitions
DEFINE_EXCEPTION(InvalidShaderException);

Shader *LoadShaderFromFile(const std::string &path, const std::string &name);
void DestroyShader(Shader *s);