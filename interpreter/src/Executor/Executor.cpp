#include "Executor.h"
#include <string>
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, std::string> variables;

bool Executor::print(std::string message)
{
	printf("%s", message.c_str());
	printf("\n");
	return true;
}

bool Executor::end()
{
	printf("\n\nEND OF EXECUTION.\nRETURNED 0");
	printf("\n");
	return true;
}

void Executor::setVariable(const std::string& name, const std::string& value)
{
	variables[name] = value;
	std::cout << "VARIABLE SET: " << name << " VALUE: " << value << std::endl;
}
