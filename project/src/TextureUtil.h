#pragma once

#include "Texture.h"
#include "Utility/Exception.h"
#include <string>

DEFINE_EXCEPTION(InvalidTextureException);

Texture *LoadTextureFromFile(const std::string &path, const std::string &name);
void DestroyTexture(Texture *t);