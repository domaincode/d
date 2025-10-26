#include "../Server.hpp"


std::string getReason(const std::vector<std::string> &command)
{

    if (command.size() < 3)
    {
        return "";
    }
    std::string result;
    for (size_t i = 3; i < command.size(); i++)
    {
        if (i > 3)
        {
            result += " ";
        }
        result += command[i];
    }
    if (!result.empty() && result[0] == ':')
    {
        result = result.substr(1);
    }
    return result;
}

std::map<std::string, std::vector<std::string > > parseKickCommand(const std::vector<std::string> &command)
{
    std::map<std::string, std::vector<std::string > > tokens;
    if (command.size() < 3)
        return tokens;

    std::vector<std::string> channelNames = split(command[1], ',');
    std::vector<std::string> users = split(command[2], ',');

    if (channelNames.size() == users.size())
    {
        for (size_t i = 0; i < channelNames.size(); ++i)
        {
            tokens[channelNames[i]].push_back(users[i]);
        }
    }
    else if (channelNames.size() == 1 && users.size() >= 1)
    {
        for (size_t i = 0; i < users.size(); ++i)
        {
            tokens[channelNames[0]].push_back(users[i]);
        }
    }
    else if (users.size() == 1 && channelNames.size() >= 1)
    {
        for (size_t i = 0; i < channelNames.size(); ++i)
        {
            tokens[channelNames[i]].push_back(users[0]);
        }
    }

    return tokens;
}

void Server::kickCommand(Client &currClient, std::string &channelName, std::string &nickname, std::string &reason)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end())
    {
        currClient.sendReply(ERR_NOSUCHCHANNEL(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        return;
    }

    Channel &currChannel = it->second;

    if (!currChannel.isOperator(currClient.Get_fd()))
    {
        currClient.sendReply(ERR_CHANOPRIVSNEEDED(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        return;
    }

    int client_fd = Get_ClientFdByName(nickname);
    Client *targetClient = (client_fd == -1 ? NULL : &_clients[client_fd]);

    if (!targetClient || !targetClient->isFullyAuthenticated())
    {
        currClient.sendReply(ERR_NOSUCHNICK(currClient.Get_hostname(), currClient.Get_nickname(), nickname));
        return;
    }

    if (!currChannel.isClientInChannel(targetClient->Get_fd()))
    {
        currClient.sendReply(ERR_USERNOTINCHANNEL(currClient.Get_nickname(), nickname, channelName));
        return;
    }

    if (!currChannel.removeClientFromChannel(targetClient->Get_fd()))
        return;

    std::string message = RPL_KICK(currClient.Get_nickname(), currClient.Get_hostname(), currClient.Get_hostname(), nickname, channelName, reason);

    targetClient->sendReply(message);
    currChannel.broadcastMessage(message, _clients);

    if (currChannel.Get_users().empty())
    {
        _channels.erase(it);
        return;
    }

    if (currChannel.Get_operators().empty())
    {
        std::vector<int> &clients = currChannel.Get_users();
        if (!clients.empty())
        {
            int newOperatorFd = clients[0];
            currChannel.addOperator(newOperatorFd);

            _clients[newOperatorFd].sendReply(RPL_YOUREOPER(_clients[newOperatorFd].Get_nickname()));
        }
    }
}

void Server::channelKick(Client &currClient, std::vector<std::string> command)
{
    if (command.size() < 3)
    {
        currClient.sendReply(ERR_NEEDMOREPARAMS(currClient.Get_nickname(), currClient.Get_hostname(), command[0]));
        return;
    }

    std::string reason = getReason(command);

    std::map<std::string, std::vector<std::string > > tokens = parseKickCommand(command);
    if (tokens.empty())
    {
        currClient.sendReply(ERR_NEEDMOREPARAMS(currClient.Get_nickname(), currClient.Get_hostname(), command[0]));
        return;
    }

    for (std::map<std::string, std::vector<std::string > >::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::string channelName = it->first;
        std::vector<std::string> users = it->second;
        for (size_t i = 0; i < users.size(); i++)
        {
            std::string nickname = users[i];
            kickCommand(currClient, channelName, nickname, reason);
        }
    }
}
