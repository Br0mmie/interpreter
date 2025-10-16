#include "Executor.h"
#include <string>

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