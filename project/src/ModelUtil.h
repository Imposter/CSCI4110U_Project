#pragma once

#include "Model.h"
#include "Utility/Exception.h"
#include <string>

DEFINE_EXCEPTION(InvalidModelException);
DEFINE_EXCEPTION(ModelLoadException);

Model *LoadModelFromFile(const std::string &path, const std::string &name);
void DestroyModel(Model *m);