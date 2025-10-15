#include "../Server.hpp"

std::string getTopic(std::vector<std::string> command)
{
    std::string result = "";
    for (size_t i = 2; i < command.size(); i++)
    {
        if (i > 2)
            result += " ";
        result += command[i];
    }
    if (!result.empty() && result[0] == ':')
        result = result.substr(1);
    return result;
}

void Server::channelTopic(Client &currClient, std::vector<std::string> command)
{
    if (command.size() < 2)
    {
        currClient.sendReply(ERR_NEEDMOREPARAMS(currClient.Get_nickname(), currClient.Get_hostname() ,command[0]));
        return;
    }

    std::string channelName = command[1];
    std::string newTopic = getTopic(command);

    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end())
    {
        currClient.sendReply(ERR_NOSUCHCHANNEL(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        return;
    }

    Channel &currChannel = it->second;

    if (!currChannel.isClientInChannel(currClient.Get_fd()))
    {
        currClient.sendReply(ERR_USERNOTINCHANNEL(currClient.Get_nickname(), currClient.Get_nickname() , channelName));
        return;
    }

    if (command.size() == 2)
    {
        if (currChannel.Get_topic().empty())
        {
            currClient.sendReply(RPL_NOTOPIC(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        }
        else
        {
            currClient.sendReply(RPL_TOPIC(currClient.Get_hostname(), currClient.Get_nickname(), channelName, currChannel.Get_topic()));
            currClient.sendReply(RPL_TOPICWHOTIME(currClient.Get_nickname(), channelName, currChannel.Get_topicSetter(), currChannel.Get_topicDate()));
        }
        return;
    }

    if (currChannel.Get_topicLock() && !currChannel.isOperator(currClient.Get_fd()))
    {
        currClient.sendReply(ERR_CHANOPRIVSNEEDED(currClient.Get_hostname(), currClient.Get_nickname(),  channelName));
        return;
    }

    //currChannel.setTopic(newTopic);
    currChannel.Get_topic() = newTopic;
    //currChannel.setTopicDate(getCurrTime());
    currChannel.Get_topicDate() = getCurrTime();
    //currChannel.setTopicSetter(currClient.Get_nickname());
    currChannel.Get_topicSetter() = currClient.Get_nickname();

    currChannel.broadcastMessage(RPL_TOPIC(currClient.Get_hostname(), currClient.Get_nickname(), channelName, newTopic), this->_clients);
}
