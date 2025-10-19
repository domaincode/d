#include "Server.hpp"

int Server::EXIT_FLAG = 0;

Server::~Server()
{
    std::cout << "Server Destructor Called\n";
    cleanup();
}

Server::Server(int port, std::string pass)
{
    //std::cout << "Server Constructor Called\n";
    this->_port = port;
    this->_password = pass;
    char hostBuffer[256];
    if (gethostname(hostBuffer, sizeof(hostBuffer)) == 0)
        _hostname = hostBuffer;
    else
        _hostname = "localhost";
}

void Server::Start()
{
    listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(listen_fd < 0)
    {
        std::cout << "Error: Func ==> Socket\n";
        exit(1);
    }
    sAddress.sin_family = AF_INET;
    sAddress.sin_port = htons(_port);
    sAddress.sin_addr.s_addr = INADDR_ANY;
    
    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("Failed to set socket option SO_REUSEADDR");
    if(bind(listen_fd, (sockaddr* )&sAddress, sizeof(sAddress)) < 0 )
        throw std::runtime_error("Failed to Bind");
    if(listen(listen_fd, 100) < 0 )
        throw std::runtime_error("Failed to Listen");
    
    pollfd fd;
    memset(&fd, 0, sizeof(fd));
    fd.events = POLLIN;
    fd.fd = listen_fd;
    _fds.push_back(fd);
    // launchBOT();

    Use_Poll();

}

void Server::Use_Poll()
{
    int j;
  
    while(true)
    {
        if(Server::EXIT_FLAG == 1)
            break;
        std::cout << "================Poll: Start========================================\n";
        j = poll(_fds.data(), _fds.size(), -1);
        if(j == -1)
        {
            continue;
        }
        std::cout << "Reads Available: " << j << std::endl;
        std::cout << "================Poll: End========================================\n";
        Check_IandO();

}

}


void Server::Check_IandO()
{
    if(_fds[0].revents == POLLIN)
    {
        Accept_NewClient();
    }
    for(unsigned int i = 1; i < _fds.size(); i++)
    {
         if(_fds[i].revents == POLLIN)
        {
             std::cout << "================Communication: Start========================================\n";
            Handle_ClientRequest(_clients[_fds[i].fd]);
            std::cout << "================Communication: End========================================\n";
        }
    }
}

void Server::Accept_NewClient()
{
        sockaddr_in clientAddress;
        socklen_t Address_size = sizeof(clientAddress);
        pollfd fd;
        memset(&fd, 0, sizeof(fd));
        fd.events = POLLIN;

        fd.fd = accept(listen_fd, (sockaddr* )&clientAddress, &Address_size);
        if(fd.fd < 0)
            return;
        fcntl(fd.fd, F_SETFL, O_NONBLOCK); 

        _fds.push_back(fd);
        Client new_client(fd.fd, this);

        new_client.Get_ip() = inet_ntoa(clientAddress.sin_addr);
        _clients[fd.fd] = new_client;
         std::cout << "Server have: " << _clients.size() << " Clients."<< std::endl;
}


void Server::Handle_ClientRequest(Client& client)
{
     char buffer[BUFFER_SIZE + 1];
    int bytes_read = recv(client.Get_fd(), buffer, BUFFER_SIZE, 0);

    if (bytes_read == 0)
    {
        std::cout << RED << "Client Disconnected: " << RESET << client.Get_fd() << "\n";
        removeClient(client.Get_fd());
    }
    else
    {
   

        buffer[bytes_read] = 0;
        client.Get_buffer().append(buffer, bytes_read);
        size_t pos;
        while (((pos = client.Get_buffer().find("\r\n")) != std::string::npos) || ((pos = client.Get_buffer().find("\n")) != std::string::npos))
        {
             size_t delim_len = (client.Get_buffer()[pos] == '\r') ? 2 : 1;
            std::string command_str = client.Get_buffer().substr(0, pos + delim_len);

            client.Get_buffer().erase(0, pos + delim_len);
            std::vector<std::string> command = split(trimString(command_str), ' ');

            std::cout << GREEN << "Received: " << RESET << command_str;

            if (command[0] == "PASS")
                client.PassCommand(command, *this);
            else if (command[0] == "NICK")
                client.NickCommand(command, *this);
            else if (command[0] == "USER")
                client.UserCommand(command);
            else if (client.isFullyAuthenticated())
            {
                if (command[0] == "JOIN")
                   ChannelJoin(client, command);
                else if (command[0] == "MODE")
                    channelMode(client, command);
                else if (command[0] == "KICK")
                    channelKick(client, command);
                else if (command[0] == "TOPIC")
                    channelTopic(client, command);
                else if (command[0] == "INVITE")
                    channelInvite(client, command);
                else if (command[0] == "PRIVMSG")
                    PrivMsgCommand(client, command, command_str);
                else if (command[0] == "SECBOT")
                    BotCommand(client.Get_fd(), command);
            }


        }
    }

    
}





