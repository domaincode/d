#include "../Server.hpp"

void Server::channelInvite(Client &currClient, std::vector<std::string> command)
{
    if (command.size() != 3)
    {
        currClient.sendReply(ERR_NEEDMOREPARAMS(currClient.Get_nickname(), currClient.Get_hostname(), "INVITE"));
        return;
    }

    std::string nickname = command[1];
    std::string channelName = command[2];

    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end())
    {
        currClient.sendReply(ERR_NOSUCHCHANNEL(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        return;
    }

    Channel &currChannel = it->second;

    if (!currChannel.isOperator(currClient.Get_fd()))
    {
        currClient.sendReply(ERR_CHANOPRIVSNEEDED(currClient.Get_hostname(), currClient.Get_nickname(),  channelName));
        return;
    }

    Client *targetClient = NULL;
    for (std::map<int, Client>::iterator it_client = _clients.begin(); it_client != _clients.end(); ++it_client)
    {
        if (it_client->second.Get_nickname() == nickname)
        {
            targetClient = &it_client->second;
            break;
        }
    }
    if (!targetClient || !targetClient->isFullyAuthenticated())
    {
        currClient.sendReply(ERR_NOSUCHNICK(currClient.Get_hostname(), currClient.Get_nickname(), nickname));
        return;
    }

    if (currChannel.isClientInChannel(targetClient->Get_fd()))
    {
        currClient.sendReply(ERR_USERONCHANNEL(currClient.Get_hostname(), currClient.Get_nickname(), nickname, channelName));
        return;
    }

    if (currChannel.isInvited(targetClient->Get_fd()))
        return;

    currChannel.addInvited(targetClient->Get_fd());

    currClient.sendReply(RPL_INVITING(currClient.Get_hostname(), currClient.Get_nickname(), nickname, channelName));
    currChannel.broadcastMessage(RPL_INVITE(currClient.Get_nickname(), currClient.Get_username(), targetClient->Get_hostname(), channelName, nickname), _clients);

}
