#include "../Server.hpp"


void Server::oModeParam(Channel &currChannel, std::string &parameter, std::string &mode, Client &currClient)
{
    if (parameter.empty())
        return;

    std::map<int, Client>::iterator it = _clients.find(Get_ClientFdByName(parameter));
    if (it == _clients.end() || !it->second.isFullyAuthenticated())
    {
        currClient.sendReply(ERR_NOSUCHNICK(currClient.Get_hostname(), currClient.Get_nickname(), parameter));
        return;
    }

    Client &targetClient = it->second;
    if (!currChannel.isClientInChannel(targetClient.Get_fd()))
    {
        currClient.sendReply(ERR_USERNOTINCHANNEL(currClient.Get_hostname(), targetClient.Get_nickname(), currChannel.Get_name()));
        return;
    }

    if (mode == "+o")
    {
        if (!currChannel.isOperator(targetClient.Get_fd()))
        {
            currChannel.addOperator(targetClient.Get_fd());
            targetClient.sendReply(RPL_YOUREOPER(targetClient.Get_nickname()));
        }
    }
    else if (mode == "-o" && currChannel.isOperator(targetClient.Get_fd()))
    {
        currChannel.removeOperator(targetClient.Get_fd());
    }

    broadcastModeChange(currClient, currChannel, mode, parameter);
}

void Server::iModeParam(Channel &currChannel, const std::string &mode, Client &currClient)
{
    currChannel.Set_inviteOnly(mode == "+i");
    broadcastModeChange(currClient, currChannel, mode, "");
}

void Server::pluskModeParam(Channel &currChannel, const std::string &parameter, Client &currClient)
{
    if (parameter.empty())
        return;

    currChannel.Set_key(parameter);
    broadcastModeChange(currClient, currChannel, "+k", parameter);
}

void Server::minuskModeParam(Channel &currChannel, Client &currClient)
{
    currChannel.Set_key("");
    broadcastModeChange(currClient, currChannel, "-k", "");
}

void Server::pluslModeParam(Channel &currChannel, const std::string &parameter, Client &currClient)
{
    if (parameter.empty())
        return;

    if (!isValidNumber(parameter))
    {
        currClient.sendReply(ERR_INVALIDMODEPARAM(currClient.Get_nickname(), currChannel.Get_name(), "+l"));
        return;
    }

    //currChannel.setUserLimit(ft_atoi(parameter));
    /// did he check the minus ??? -12;
    currChannel.Get_userLimit() = ft_atoi(parameter);
    broadcastModeChange(currClient, currChannel, "+l", parameter);
}

void Server::minuslModeParam(Channel &currChannel, Client &currClient)
{
   // currChannel.setUserLimit(0);
   currChannel.Get_userLimit() = 0;
    broadcastModeChange(currClient, currChannel, "-l", "");
}


void Server::tModeParam(Channel &currChannel, const std::string &mode, Client &currClient)
{
    currChannel.Set_topicLock(mode == "+t");
    broadcastModeChange(currClient, currChannel, mode, "");
}

void Server::broadcastModeChange(Client &currClient, Channel &currChannel, const std::string &mode, std::string parameter)
{
    std::string modeMessage = ":" + currClient.Get_Prefix() + " MODE " + currChannel.Get_name() + " " + mode;
    if (!parameter.empty())
        modeMessage += " " + parameter;
    modeMessage += "\r\n";

    currChannel.broadcastMessage(modeMessage, _clients);
}

void Server::sendChannelInfo(Client &currClient, Channel &currChannel)
{
    currClient.sendReply(RPL_CHANNELMODEIS(_hostname, currClient.Get_nickname(), currChannel.Get_name(), currChannel.Get_ChannelModes()));
    currClient.sendReply(RPL_CREATIONTIME(_hostname, currClient.Get_nickname(), currChannel.Get_name(), currChannel.Get_creationDate()));
}



void Server::channelMode(Client &currClient, std::vector<std::string> command)
{
    if (command.size() < 2)
    {
        currClient.sendReply(ERR_NEEDMOREPARAMS(currClient.Get_nickname(), currClient.Get_hostname(), command[0]));
        return;
    }

    std::string channelName = command[1];
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end())
    {
        currClient.sendReply(ERR_NOSUCHCHANNEL(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        return;
    }

    Channel &currChannel = it->second;

    if (command.size() == 2)
    {
        sendChannelInfo(currClient, currChannel);
        return;
    }

    if (!currChannel.isOperator(currClient.Get_fd()))
    {
        currClient.sendReply(ERR_CHANOPRIVSNEEDED(currClient.Get_hostname(), currClient.Get_nickname(), channelName));
        return;
    }

    std::vector<std::string> modes;
    //if ri zayda
    // if (command.size() >= 3)
    // {
    // MODE #TESTO +i +t
        if (command[2][0] != '+' && command[2][0] != '-')
            return;
        modes = parseModes(command[2]);
    // }
    
    std::vector<std::string> parameters = parseParametres(command, modes);
    if (allParamEmpty(parameters, modes))
    {
        sendChannelInfo(currClient, currChannel);
        return;
    }

    int paramCount = 0;
    for (size_t i = 0; i < modes.size(); i++)
    {
        if ((modes[i] == "+o" || modes[i] == "-o"))
        {
            oModeParam(currChannel, parameters[paramCount++], modes[i], currClient);
        }
        else if (modes[i] == "+i" || modes[i] == "-i")
        {
            iModeParam(currChannel, modes[i], currClient);
        }
        else if (modes[i] == "+k")
        {
            pluskModeParam(currChannel, parameters[paramCount++], currClient);
        }
        else if (modes[i] == "-k")
        {
            minuskModeParam(currChannel, currClient);
        }
        else if (modes[i] == "+l")
        {
            pluslModeParam(currChannel, parameters[paramCount++], currClient);
        }
        else if (modes[i] == "-l")
        {
            minuslModeParam(currChannel, currClient);
        }
        else if (modes[i] == "+t" || modes[i] == "-t")
        {
            tModeParam(currChannel, modes[i], currClient);
        }
        else
        {
            currClient.sendReply(ERR_UNKNOWNMODE(currClient.Get_hostname(), currClient.Get_nickname(), modes[i]));
        }
    }
}
