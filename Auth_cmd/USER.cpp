#include "../Client.hpp"


void Client::UserCommand(std::vector<std::string> command)
{
    if (command.size() < 5 || command[4][0] != ':')
    {
        sendReply(ERR_NEEDMOREPARAMS(_nickname, _hostname, "USER"));
        return;
    }

    if (_authStatus != 0x01 && _authStatus != 0x03)
    {
        if (_authStatus == 0x07)
            sendReply(ERR_ALREADYREGISTERED(_nickname, _hostname));
        return;
    }

    std::string username = command[1];
    std::string servername = command[3];
    std::string fullname = command[4];

    if (fullname[0] == ':')
    {
        fullname = fullname.substr(1);
        for (size_t i = 5; i < command.size(); ++i)
        {
            fullname += " " + command[i];
        }
    }

    // currClient.setUsername(username);
    // currClient.setServername(servername);
    // currClient.setRealname(realname);
    // currClient.setRegistered(true);
    // currClient.setClientFd(client_fd);
    // currClient.setAuthStatus(0x04);

    _username = username;
    _servername = servername;
    _fullname = fullname;
    _registered = true;
    // ???? set authstatus _authstatus |= new_authstatus
    _authStatus |= 0x04;

    if (_authStatus == 0x07)
    {
        sendWelcomeMessages();
        _nickFlag = 1;
    }
}