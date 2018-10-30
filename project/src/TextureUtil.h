#pragma once

#include "Texture.h"
#include <string>

Texture *LoadTextureFromFile(const std::string &fileName, Texture::Format format = Texture::kFormat_RGB);