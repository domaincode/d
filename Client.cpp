#include "Client.hpp"
// #include "Server.hpp"

Client::Client(){   std::cout << "Default Constructor Client Called\n";}

Client::~Client(){ std::cout << "Destructor Client Called\n";}
    

Client::Client(int fd,  Server* server)
{
    std::cout << "Parameter Constructor Client Called\n";
    this->_fd = fd;
    this->_server = server;
    this->is_authenticated = false;
    this->authenticate_level = 0;
}


Client::Client(const Client& other)
{
    std::cout << "Copy constructor Client Called\n";
    *this = other;
}

void Client::Set_buffer(std::string msg)
{
    _buffer = msg;
}



Client& Client::operator=(const Client& other)
{
    std::cout << "Assignement operator Client Called\n";

    if(this == &other)
    {
        return *this;
    }
    this->_fd = other._fd;
    this->_server = other._server;
    this->is_authenticated = other.is_authenticated;
    this->authenticate_level = other.authenticate_level;

    return *this;
}



