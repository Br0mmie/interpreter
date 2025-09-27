#include <iostream>
#include <fstream>
#include <vector>
#include "Utils/File.h"
#include "Tokenize/Tokenizer.h"

int main(int argc, char* argv[])
{
	std::ifstream file = File::OpenFile(argv[1]);

	std::vector<std::string> lines = File::ReadFile(file);

	std::vector<std::string> tokens = Tokenizer::ReadLines(lines);

	Tokenizer::ReadTokens(tokens);

	std::cin.get();
}