#include "Client.hpp"


std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delimiter);
	while (end != std::string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}
	tokens.push_back(str.substr(start));
	return tokens;
}

std::string trimString(const std::string &input)
{
	std::string result;
	size_t i = 0;

	while (i < input.size() && isspace(input[i]))
		i++;

	bool inSpace = false;
	while (i < input.size())
	{
		if (isspace(input[i]))
		{
			if (!inSpace)
			{
				result += ' ';
				inSpace = true;
			}
		}
		else
		{
			result += input[i];
			inSpace = false;
		}
		i++;
	}

	if (!result.empty() && isspace(result[result.size() - 1]))
		result.erase(result.size() - 1);

	return result;
}