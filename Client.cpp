#include "Client.hpp"

Client::Client()
{
    char hostname[100];
    if(gethostname(hostname, 100) < 0)
        _hostname = "localhost";
    else
        _hostname = hostname;


}

Client::~Client(){}
    

Client::Client(int fd,  Server* server)
{
    this->_fd = fd;
    this->_server = server;
    this->is_authenticated = false;
    _authStatus = 0;
    _nickFlag = 0;
    char hostname[100];
    if(gethostname(hostname, 100) < 0)
        _hostname = "localhost";
    else
        _hostname = hostname;
}


Client::Client(const Client& other)
{
    *this = other;
}



Client& Client::operator=(const Client& other)
{
    if(this == &other)
    {
        return *this;
    }
    _fd = other._fd;
    _server = other._server;
    is_authenticated = other.is_authenticated;
    _authStatus = other._authStatus;
    _nickFlag = other._nickFlag;
    _hostname = other._hostname;
    return *this;
}



