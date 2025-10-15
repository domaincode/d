#include "Client.hpp"
// #include "Server.hpp"

Client::Client()
{
    char hostname[100];
    std::cout << "Default Constructor Client Called\n";
    if(gethostname(hostname, 100) < 0)
        _hostname = "localhost";
    else
        _hostname = hostname;


}

Client::~Client(){ std::cout << "Destructor Client Called\n";}
    

Client::Client(int fd,  Server* server)
{
    std::cout << "Parameter Constructor Client Called\n";
    this->_fd = fd;
    this->_server = server;
    this->is_authenticated = false;
    _authStatus = 0;
    _nickFlag = 0;
}


Client::Client(const Client& other)
{
    std::cout << "Copy constructor Client Called\n";
    *this = other;
}



Client& Client::operator=(const Client& other)
{
    std::cout << "Assignement operator Client Called\n";

    if(this == &other)
    {
        return *this;
    }
    _fd = other._fd;
    _server = other._server;
    is_authenticated = other.is_authenticated;
    _authStatus = other._authStatus;
    _nickFlag = other._nickFlag;
    return *this;
}



