#include "../Core.h"
#include "File.h"

std::ifstream File::OpenFile(const std::string& filename)
{
	std::ifstream file(filename);
	return file;
}

std::vector<std::string> File::ReadFile(std::ifstream& file)
{
	int lineCounter = 0;
	if (!file.is_open())
	{
		std::cerr << "Error: File could not be opened." << std::endl;
		return {};
	}

	std::string tempString;
	std::vector<std::string> lines;
	while (std::getline(file, tempString))
	{
		// std::cout << lineCounter << " " << tempString << std::endl;

		lineCounter++;
		lines.push_back(tempString);
	}
	return lines;
}