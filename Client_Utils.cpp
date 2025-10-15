#include "Client.hpp"


std::string& Client::Get_buffer()
{
    return _buffer;
}

int& Client::Get_fd()
{ 
    return _fd;
}

std::string& Client::Get_ip()
{
    return _ip;
}



bool Client::isFullyAuthenticated()
{
    return (_authStatus & 0x07) == 0x07;
}

int& Client::get_auth_level()
{
    return authenticate_level;
}


std::string& Client::Get_nickname()
{
    return _nickname;
}


void Client::sendReply(std::string response)
{
    if (send(_fd, response.c_str(), response.length(), 0) == -1)
        return;
}


std::string& Client::Get_username()
{
    return _username;
}

std::string& Client::Get_fullname()
{
    return _fullname;
}


std::string& Client::Get_hostname()
{
    return _hostname;
}

int& Client::Get_authStatus()
{
    return _authStatus;
}

int& Client::Get_nickFlag()
{
    return _nickFlag;
}


std::string Client::Get_Prefix() const
{
    return _nickname + "!" + _username + "@" + _hostname;
}
