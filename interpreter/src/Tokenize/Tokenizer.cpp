#include "Tokenizer.h"
#include <iostream>
#include <vector>
#include "../Executor/Executor.h"

std::vector<std::string> Tokenizer::m_Strings;

std::vector<std::string> Tokenizer::ReadLines(std::vector<std::string> lines)
{
	std::vector<std::string> tokens;
	for (const auto& line : lines)
	{
		std::string current;
		bool inString = false;
		for (size_t i = 0; i < line.size(); ++i)
		{
			char ch = line[i];
			if (inString)
			{
				if (ch == '"')
				{
					m_Strings.push_back(current);
					current.clear();
					inString = false;
				}
				else
				{
					current += ch;
				}
			}
			else
			{
				if (ch == ' ')
				{
					if (!current.empty())
					{
						tokens.push_back(current);
						current.clear();
					}
				}
				else if (ch == '"')
				{
					if (!current.empty())
					{
						tokens.push_back(current);
						current.clear();
					}
					inString = true;
				}
				else
				{
					current += ch;
				}
			}
		}
		if (!current.empty())
		{
			if (inString)
				m_Strings.push_back(current);
			else
				tokens.push_back(current);
		}
	}
	return tokens;
}

bool Tokenizer::ReadTokens(std::vector<std::string> tokens)
{
	for (const auto& token : tokens)
	{
		if (token == "PRINT")
		{
			// std::cout << "PRINT STATEMENT FOUND" << std::endl;
			Executor::print(m_Strings[0]);
		}
		else if (token == "END")
		{
			// std::cout << "END STATEMENT FOUND" << std::endl;
			Executor::end();
		}
		else if (token == "LET")
		{
			// start of variable found

		}
		else
		{
			// No action for other tokens
		}
	}
	return true;
}
