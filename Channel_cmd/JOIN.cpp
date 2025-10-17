#include "../Server.hpp"

bool isValidChannelName(const std::string &name)
{
	if (name.empty() || (name[0] != '#' && name[0] != '&'))
		return false;
	// if (name.find(',') != std::string::npos)
	// 	return false;
	return name.length() <= 200;
}


std::map<std::string, std::string> parseJoinCommand(std::vector<std::string> command)
{
	std::map<std::string, std::string> tokens;
	std::vector<std::string> keys;
	std::vector<std::string> channelsNames;

	// if (command.size() >= 2)
	// {
		channelsNames = split(command[1], ',');
		if (command.size() >= 3)
		{
			keys = split(command[2], ',');
		}

		for (size_t i = 0; i < channelsNames.size(); i++)
		{
			if (i < keys.size() && !keys[i].empty())
			{
				tokens[channelsNames[i]] = keys[i];
			}
			else
			{
				tokens[channelsNames[i]] = "";
			}
		}
	// }

	return tokens;
}

void Server::joinCommand(std::string channelName, std::string key, Client &client)
{
    //int client_fd = client.Get_fd();
    std::map<std::string, Channel>::iterator it;
    it = _channels.find(channelName);
    if (it == _channels.end())
    {
        if (!isValidChannelName(channelName))
            return;
        Channel newChannel(channelName, key);
        newChannel.addClient(client.Get_fd());
        newChannel.addOperator(client.Get_fd());

        _channels[channelName] = newChannel;
        std::string message = RPL_JOIN(client.Get_nickname(), client.Get_username(), channelName, client.Get_ip());
        client.sendReply(message);
        if (newChannel.Get_topic() == "")
            client.sendReply(RPL_NOTOPIC(client.Get_hostname(), client.Get_nickname(), channelName));
        // else
        // {
        //     client.sendReply(RPL_TOPIC(client.Get_hostname(), client.Get_nickname(), channelName, newChannel.Get_topic()));
        //     client.sendReply(RPL_TOPICWHOTIME(client.Get_nickname(), channelName, newChannel.Get_topicSetter(), newChannel.Get_topicDate()));
        // }
        std::cout << "start\n";
        std::cout << newChannel.getAllUsersNames(_clients);
        std::cout << "end\n";
        client.sendReply(RPL_NAMREPLY(client.Get_hostname(), newChannel.getAllUsersNames(_clients) , channelName, client.Get_nickname()));
        client.sendReply(RPL_ENDOFNAMES(client.Get_hostname(), client.Get_nickname(), channelName));
    }
    else
    {
        Channel &currChannel = it->second;
        //std::cout << "invite only: " << currChannel.Get_inviteOnly() << std::endl;
        if (currChannel.Get_inviteOnly() && !currChannel.isInvited(client.Get_fd()))
        {
            client.sendReply(ERR_INVITEONLYCHAN(client.Get_nickname(), channelName));
            return;
        }

        if (currChannel.Get_userLimit() != 0 && currChannel.Get_userCount() >= currChannel.Get_userLimit())
        {
            client.sendReply(ERR_CHANNELISFULL(client.Get_hostname(), client.Get_nickname(), channelName));
            return;
        }

        if (currChannel.Get_key() != key)
        {
            client.sendReply(ERR_BADCHANNELKEY(client.Get_nickname(), client.Get_hostname(), channelName));
            return;
        }

        if (std::find(currChannel.Get_users().begin(), currChannel.Get_users().end(), client.Get_fd()) != currChannel.Get_users().end())
        {
            client.sendReply(ERR_USERONCHANNEL(client.Get_hostname(), client.Get_nickname(), client.Get_nickname(), channelName));
            return;
        }
        //////must be removed from the invite only list ???
        currChannel.addClient(client.Get_fd());
        std::string message = RPL_JOIN(client.Get_nickname(), client.Get_username(), channelName, client.Get_ip());
        currChannel.broadcastMessage(message, _clients);
        if (currChannel.Get_topic() == "")
            client.sendReply(RPL_NOTOPIC(client.Get_hostname(), client.Get_nickname(), channelName));
        else
        {
            client.sendReply(RPL_TOPIC(client.Get_hostname(), client.Get_nickname(), channelName, currChannel.Get_topic()));
            client.sendReply(RPL_TOPICWHOTIME(client.Get_nickname(), channelName, currChannel.Get_topicSetter(), currChannel.Get_topicDate()));
        }
        client.sendReply(RPL_NAMREPLY(client.Get_hostname(), currChannel.getAllUsersNames(_clients), channelName, client.Get_nickname()));
        client.sendReply(RPL_ENDOFNAMES(client.Get_hostname(), client.Get_nickname(), channelName));
    }
}

void Server::ChannelJoin(Client &client, std::vector<std::string> command)
{
    if (command.size() < 2)
    {
        client.sendReply(ERR_NEEDMOREPARAMS(client.Get_nickname(), client.Get_hostname(), command[0]));
        return;
    }
    std::map<std::string, std::string> tokens = parseJoinCommand(command);
    // if (tokens.size() == 0)
    // {
    //     client.sendReply(ERR_NEEDMOREPARAMS(client.Get_nickname(), client.Get_hostname(), command[0]));
    //     return;
    // }

    std::map<std::string, std::string>::iterator it;
    for (it = tokens.begin(); it != tokens.end(); it++)
    {
        joinCommand(it->first, it->second, client);
    }
}
