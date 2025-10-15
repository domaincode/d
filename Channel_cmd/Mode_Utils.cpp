#include "../Server.hpp"


int ft_atoi(std::string parameter)
{
    size_t i = 0;
    int signe = 1;
    unsigned long long total = 0;

    while (parameter[i] >= 9 && parameter[i] <= 13)
        i++;
    if (parameter[i] == '+' || parameter[i] == '-')
    {
        if (parameter[i] == '-')
            signe = -1;
        i++;
    }
    while (parameter[i] >= '0' && parameter[i] <= '9')
    {
        if (total > (9223372036854775807ULL - (parameter[i] - '0')) / 10)
            return (signe == 1 ? -1 : 0);
        total = total * 10 + (parameter[i] - '0');
        i++;
    }
    return ((int)(signe * total));
}

bool isValidNumber(const std::string &number)
{
    int start = 0;

    if (number[0] == '+' || number[0] == '-')
        start = 1;

    for (size_t i = start; i < number.size(); i++)
    {
        if (number[i] < '0' || number[i] > '9')
            return false;
    }

    return true;
}


std::vector<std::string> parseModes(const std::string &modes)
{
    
    std::vector<std::string> result;
    bool give = true;
    for (size_t i = 0; i < modes.size(); i++)
    {
        if (modes[i] == '+')
            give = true;
        else if (modes[i] == '-')
            give = false;
        else
            result.push_back((give ? "+" : "-") + std::string(1, modes[i]));
    }
    return result;
}

std::vector<std::string> parseParametres(std::vector<std::string> command, std::vector<std::string> modes)
{
    std::vector<std::string> result;
    size_t paramIndex = 3;

    for (size_t i = 0; i < modes.size(); i++)
    {
        if (modes[i] == "+o" || modes[i] == "-o" || modes[i] == "+l" || modes[i] == "+k")
        {
            if (paramIndex < command.size())
                result.push_back(command[paramIndex++]);
            else
                result.push_back("");
        }
    }

    return result;
}


bool allParamEmpty(std::vector<std::string> parameters, std::vector<std::string> modes)
{
    size_t paramCount = 0;
    for (size_t i = 0; i < modes.size(); i++)
    {
        if (modes[i] == "+o" || modes[i] == "-o" || modes[i] == "+l" || modes[i] == "+k")
            paramCount++;
    }
    if (paramCount != 0)
    {
        size_t total = 0;
        for (size_t i = 0; i < paramCount; i++)
        {
            if (parameters[i] == "")
                total++;
        }
        if (total == paramCount)
            return true;
    }

    return false;
}