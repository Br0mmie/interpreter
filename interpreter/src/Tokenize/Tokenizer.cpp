#include "../Core.h"
#include "Tokenizer.h"
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
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		const auto& token = tokens[i];
		if (token == "PRINT")
		{
			Executor::print(m_Strings[0]);
		}
		else if (token == "END")
		{
			Executor::end();
		}
		else if (token == "LET")
		{
			// start of variable found

		}
		else
		{
			if (i + 2 < tokens.size())
			{
				std::string varName = tokens[i + 1];
				std::string varValue = tokens[i + 2];
				Executor::setVariable(varName, varValue);
				i += 2;
			}
		}
		// ... other tokens
	}
	return true;
}