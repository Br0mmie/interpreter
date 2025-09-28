#pragma once
#include <string>
class Executor
{
public:
	static bool print(std::string message);
	static bool end();
	static void setVariable(const std::string& name, const std::string& value);
};

