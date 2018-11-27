#include "FileUtil.h"
#include <fstream>

bool File::Exists(const std::string &path)
{
	return std::ifstream(path).is_open();
}

std::vector<std::string> File::ReadAllLines(const std::string &path)
{
	std::ifstream file(path);
	if (!file.is_open())
		THROW_EXCEPTION(FileNotFoundException, "File not found: %s", path.c_str());

	std::vector<std::string> result;
	std::string line;
	while (getline(file, line)) 
		result.push_back(line);
	
	return result;
}
