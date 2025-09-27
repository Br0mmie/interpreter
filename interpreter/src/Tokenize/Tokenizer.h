#pragma once
#include <vector>
#include <string>

class Tokenizer
{
public:
	static std::vector<std::string> ReadLines(std::vector<std::string> lines);
	static bool ReadTokens(std::vector<std::string> tokens);

private:
	static std::vector<std::string> m_Strings;
};

