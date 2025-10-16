#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class File
{
public:
	static std::ifstream OpenFile(const std::string& filename);
	static std::vector<std::string> ReadFile(std::ifstream& file);
};