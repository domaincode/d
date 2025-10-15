#include "Server.hpp"


Server& Client::Get_serverObject()
{
    return *_server;
}


std::string& Server::Get_password()
{
    return _password;
}

std::map<int, Client>& Server::Get_clients_array()
{
    return _clients;
}



bool Server::Nick_isDuplicated(std::string& Nickname)
{
    for(std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if(Nickname == it->second.Get_nickname())
            return true;
    }
    return false;
}



void Server::print_clients()
{
    for(int i = 0; i < _clients.size(); i++)
    {
        std::cout << "Client: " << _clients[_fds[i].fd].Get_fd();
        std::cout << " NICK: " << _clients[_fds[i].fd].Get_nickname() << std::endl;

    }
}

int Server::Get_ClientFdByName(const std::string &nickname)
{
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.Get_nickname() == nickname)
            return it->first;
    }
    return -1;
}