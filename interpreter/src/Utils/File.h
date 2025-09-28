#pragma once
#include "../Core.h"

class File
{
public:
	static std::ifstream OpenFile(const std::string& filename);
	static std::vector<std::string> ReadFile(std::ifstream& file);
};

