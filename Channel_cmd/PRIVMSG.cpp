#include "../Server.hpp"

// int Server::getClientByNickname(const std::string &nickname) const
// {
//     for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
//     {
//         if (it->second.Get_nickname() == nickname)
//             return it->first;
//     }
//     return -1;
// }

void Server::broadcastToChannel(Client &client, const std::string &channel_name, const std::string &message)
{
    std::map<std::string, Channel>::iterator channel_it = _channels.find(channel_name);
    if (channel_it == _channels.end())
    {
        client.sendReply(ERR_NOSUCHCHANNEL(client.Get_hostname(), client.Get_nickname(), channel_name));
        return;
    }

    Channel &channel = channel_it->second;
    std::vector<int> &clients_in_channel = channel.Get_users();

    if (std::find(clients_in_channel.begin(), clients_in_channel.end(), client.Get_fd()) == clients_in_channel.end())
    {
        client.sendReply(ERR_CANNOTSENDTOCHAN(client.Get_hostname(), client.Get_nickname(), channel_name));
        return;
    }

    std::string formatted_msg = ":" + client.Get_nickname() + "!" + client.Get_username() + "@" + client.Get_hostname() + " PRIVMSG " + channel_name + " :" + message + "\r\n";
    for (std::vector<int>::iterator it = clients_in_channel.begin(); it != clients_in_channel.end(); ++it)
    {
        if (*it != client.Get_fd())
        {
            std::map<int, Client>::iterator target_client = _clients.find(*it);
            if (target_client != _clients.end())
            {
                target_client->second.sendReply(formatted_msg);
            }
        }
    }
}

void Server::sendToClient(const std::string &target_nick, Client &client, const std::string &message)
{
    int target_fd = Get_ClientFdByName(target_nick);
    if (target_fd != -1 && _clients[target_fd].isFullyAuthenticated())
    {
        std::string formatted_msg = PRIVMSG_FORMAT(client.Get_nickname(), client.Get_username(), client.Get_hostname(), target_nick, message);
        //sendReply(target_fd, formatted_msg);
        _clients[target_fd].sendReply(formatted_msg);
    }
    else
    {
        client.sendReply(ERR_NOSUCHNICK(client.Get_hostname(), client.Get_nickname(), target_nick));
    }
}

void Server::PrivMsgCommand(Client &client, std::vector<std::string> command, std::string &buffer)
{
    if (command.size() < 3)
    {
        client.sendReply(ERR_NEEDMOREPARAMS(client.Get_nickname(), client.Get_hostname(), "PRIVMSG"));
        return;
    }
    std::string target = command[1];
    std::string message = command[2];
    if (message[0] == ':')
        message = buffer.substr(buffer.find(':') + 1);
    message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
    message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
    std::string sender_nick = client.Get_nickname();
    std::vector<std::string> target_list = split(target, ',');

    for (size_t i = 0; i < target_list.size(); ++i)
    {
        std::string target = target_list[i];
        if (target == "SECBOT")
        {
            std::vector<std::string> bots_msg;
            bots_msg.push_back("SECBOT");
            bots_msg.push_back(message);
           // BotCommand(client.Get_fd(), bots_msg);
        }

        else if (target[0] == '#' || target[0] == '&')
        {
            std::map<std::string, Channel>::iterator channel_it = _channels.find(target);
            if (channel_it == _channels.end())
                client.sendReply(ERR_NOSUCHCHANNEL(client.Get_hostname(), client.Get_nickname(), target));
            else if (!channel_it->second.isClientInChannel(client.Get_fd()))
                client.sendReply(ERR_NOTONCHANNEL(client.Get_hostname(), target));
            else
                broadcastToChannel(client, target, message);
        }
        else
            sendToClient(target, client, message);
    }
}