#include "Channel.hpp"

bool Channel::isClientInChannel(int client_fd)
{
    return std::find(_users.begin(), _users.end(), client_fd) != _users.end();
}

bool Channel::isInvited(int& client_fd)
{
    for(size_t i = 0; i < _invited_users.size(); i++)
    {
        if(client_fd == _invited_users.at(i))
            return true;
    }
    return false;
}


bool Channel::isOperator(int& client_fd)
{
    for(size_t i = 0; i < _operators.size(); i++)
    {
        if(client_fd == _operators.at(i))
            return true;
    }
    return false;
}

std::string Channel::getAllUsersNames(std::map<int, Client>& _clients)
{
    std::string result;

    for (size_t i = 0; i < _users.size(); i++)
    {
        int client_fd = _users[i];

        std::map<int, Client>::iterator it = _clients.find(client_fd);

        if (it == _clients.end())
            continue;

        if (isOperator(client_fd))
            result += "@" + it->second.Get_nickname() + " ";
        else
            result += it->second.Get_nickname() + " ";
    }

    return result;
}


void Channel::broadcastMessage(std::string message, std::map<int, Client>& _clients)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		int client_fd = _users[i];

        std::map<int, Client>::iterator it = _clients.find(client_fd);
        if (it != _clients.end())
			it->second.sendReply(message);
	}
}


std::string getCurrTime(void)
{
	std::stringstream convert;
	convert << time(NULL);
	return (convert.str());
}