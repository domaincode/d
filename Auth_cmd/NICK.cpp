#include "../Client.hpp"


void Client::broadcastNickChange(const std::string &oldNick, const std::string &newNick)
{
    std::string message = ":" + oldNick + "!" + Get_username() + "@" + Get_hostname() + " NICK :" + newNick + "\r\n";

    for (std::map<int, Client>::iterator it = _server->Get_clients_array().begin(); it != _server->Get_clients_array().end(); ++it)
        it->second.sendReply(message);
}

bool isValidNickname(std::string nickname)
{
    if(nickname[0] == '-')
        return false;
    for (size_t i = 0; i < nickname.size(); i++)
    {
        if (!isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-')
            return false;
    }
    return true;
}

void Client::NickCommand(std::vector<std::string> command, Server& server)
{
    if (command.size() < 2)
    {
        sendReply(ERR_NONICKNAMEGIVEN(Get_nickname(), Get_hostname()));
        return;
    }
    if (Get_authStatus() < 0x01)
        return;

    std::string nickname = command[1];

    if (!isValidNickname(nickname))
    {
        sendReply(ERR_ERRONEUSNICKNAME(Get_nickname(), nickname, Get_hostname()));
        return;
    }

    for (std::map<int, Client>::iterator it = server.Get_clients_array().begin(); it != server.Get_clients_array().end(); ++it)
    {
        if (it->second.Get_nickname() == nickname)
        {
            sendReply(ERR_NICKNAMEINUSE(nickname));
            return;
        }
    }

    std::string oldNick = Get_nickname();
    Get_authStatus() |= 0x02;    

    //currClient.setNickname(nickname);
    Get_nickname() = nickname;

    if (Get_authStatus() == 0x07 && Get_nickFlag() == 0)
    {
        sendWelcomeMessages();
        Get_nickFlag() = 1;
   }

    if (!oldNick.empty())
        broadcastNickChange(oldNick, nickname);
}
