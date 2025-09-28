#include "../Core.h"
#include "Executor.h"

static std::unordered_map<std::string, std::string> variables;

bool Executor::print(std::string message)
{
	printf("%s", message.c_str());
	printf("\n");
	return true;
}

// Uses unordered map to combine a name string from the tokenizer
// and a value
// TODO: Optimize this
void Executor::setVariable(const std::string& name, const std::string& value)
{
	variables[name] = value;
	std::cout << "VARIABLE SET: " << name << " VALUE: " << value << std::endl;
}

// Should end all execution.
// TODO: Check if we should return 0 or 1
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
