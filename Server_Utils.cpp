#include "Server.hpp"

// void Server::cleanup()
// {
//     for (int i = 1; i < _client_count; i++)
//         removeClient(fds[i].fd);

//     if (_server_fd != -1)
//         close(_server_fd);

//     _clients.clear();
//     _channels.clear();
// }


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
    for(size_t i = 0; i < _clients.size(); i++)
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


void Server::removeClient(int client_fd)
{
    std::map<int, Client>::iterator client_it = _clients.find(client_fd);
    if (client_it == _clients.end())
        return;

    std::map<std::string, Channel>::iterator tmp;
    for (std::map<std::string, Channel>::iterator it_chan = _channels.begin(); it_chan != _channels.end();)
    {
        tmp = it_chan;
        if (it_chan->second.isClientInChannel(client_fd))
        {
            it_chan->second.removeClientFromChannel(client_fd);
            std::string message = RPL_KICK(client_it->second.Get_nickname(), client_it->second.Get_hostname(), client_it->second.Get_hostname(), client_it->second.Get_nickname(), it_chan->second.Get_name(), "");
            it_chan->second.broadcastMessage(message, _clients);
        }

        if (it_chan->second.Get_users().empty())
        {
            tmp++;
            _channels.erase(it_chan);
            it_chan = tmp;
        }
        else
            ++it_chan;
    }

    close(client_fd);
    _clients.erase(client_it);

    for (size_t i = 1; i < _fds.size(); i++)
    {
        if (_fds[i].fd == client_fd)
        {
  //         _fds[i].fd *= -1;
            if(_fds.size() > 2)
                std::swap(_fds[i], *(_fds.end()--));
            _fds.pop_back();
            break;
        }
    }
}