#include "Server.hpp"

Server::~Server()
{
    std::cout << "Server Destructor Called\n";
}

Server::Server(int port, std::string pass)
{
    std::cout << "Server Constructor Called\n";
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
    
    std::cout << "Bind: " ;
    std::cout << (bind(listen_fd, (sockaddr* )&sAddress, sizeof(sAddress)) < 0 ? "Failed\n": "Success\n");

    //// Back log find the best value ????
    std::cout << "Listen: " ;
    std::cout << (listen(listen_fd, 2) < 0 ? "Failed\n": "Success\n");
    
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
        //must set all the pollfd members 
        std::cout << "================Poll: Start========================================\n";
        j = poll(_fds.data(), _fds.size(), -1);
        if(j == -1)
        {
            std::cout << "error: in poll\n";
            exit(0);
        }
        else
        {
            std::cout << "Reads available: " << j << std::endl;
        }
        std::cout << "================Poll: End========================================\n";
        Check_IandO();

        //delete
        if(_fds.size() == 4)
            break;

}

}


void Server::Check_IandO()
{
    if(_fds[0].revents == POLLIN)
    {
        Accept_NewClient();
    }
    //Handle Client Request
    for(unsigned int i = 1; i < _fds.size(); i++)
    {
        if(_fds[i].revents == POLLIN)
        {
            std::cout << "================Communication: Start========================================\n";
            std::cout << "Client n: " << _fds[i].fd << std::endl;
           // std::cout << "size of map " << clients.size() << std::endl;
            Handle_ClientRequest(_clients[_fds[i].fd]);
            //Start_communication(clients[_fds[i].fd]);
            std::cout << "================Communication: End========================================\n";
        }
    }
}

void Server::Accept_NewClient()
{
        //New Client in the queue ???
        sockaddr_in clientAddress;
        socklen_t Address_size = sizeof(clientAddress);
        pollfd fd;
        memset(&fd, 0, sizeof(fd));
        fd.events = POLLIN;
        //POLLIN

        fd.fd = accept(listen_fd, (sockaddr* )&clientAddress, &Address_size);
        std::cout << "/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\Accept: " << fd.fd << " is connecting.\n";
        if(fd.fd < 0)
        {
            std::cout << "Error: in accept\n";
            exit(1);
        }

        // Must be check: the preiveus flags for socket ????? New client Flags ??
        fcntl(fd.fd, F_SETFL, O_NONBLOCK); //fcntl Non Blocking I/O

        //add the new client to the array for monitoring
        _fds.push_back(fd);
        Client new_client(fd.fd, this);

        // Client must have Default Constructor
        new_client.Get_ip() = inet_ntoa(clientAddress.sin_addr);
        _clients[fd.fd] = new_client;
         std::cout << "Server have: " << _clients.size() << " Clients."<< std::endl;
      




}


void Server::Handle_ClientRequest(Client& client)
{
     char buffer[BUFFER_SIZE + 1];
    int bytes_read = recv(client.Get_fd(), buffer, BUFFER_SIZE, 0);
    std::cout << "bytes read: " << bytes_read << std::endl;

    if (bytes_read == 0)
    {
        std::cout << "Client disconnected." << std::endl;
        removeClient(client.Get_fd());
        return;
    }
    else
    {
        buffer[bytes_read] = 0;
        client.Get_buffer().append(buffer, bytes_read);
        size_t pos;
        while (((pos = client.Get_buffer().find("\r\n")) != std::string::npos) || ((pos = client.Get_buffer().find("\n")) != std::string::npos))
        {
            //std::string command_str = client.Get_buffer().substr(0, pos + 2);
            std::string command_str = client.Get_buffer();
            client.Get_buffer().erase(0, pos + 2);
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





