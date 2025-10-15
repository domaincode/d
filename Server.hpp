#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <algorithm>

#include <poll.h>




class Channel;
class Client;

class Server
{
    private:
        std::string _password; 
        int _port;
        std::string _hostname;
        std::map<int, Client> _clients;
        std::map<std::string, Channel> _channels;
        std::vector<pollfd> _fds;
        int listen_fd;
        sockaddr_in sAddress;


    public:
        Server(int port);
        ~Server();
        void Start();


        std::string& Get_password();
        int Get_ClientFdByName(const std::string &nickname);
        std::map<int, Client>& Get_clients_array();

        void Use_Poll();
        void Check_IandO();
        void Accept_NewClient();
        void Handle_ClientRequest(Client& client);


        //channel commands
        void channelKick(Client &currClient, std::vector<std::string> command);
        void kickCommand(Client &currClient, std::string &channelName, std::string &nickname, std::string &reason);

        void channelTopic(Client &currClient, std::vector<std::string> command);

        void channelInvite(Client &currClient, std::vector<std::string> command);

        void PrivMsgCommand(Client &client, std::vector<std::string> command, std::string &buffer);
        void broadcastToChannel(Client &client, const std::string &channel_name, const std::string &message);
        void sendToClient(const std::string &target_nick, Client &client, const std::string &message);






        //Mode command
        void pluskModeParam(Channel &currChannel, const std::string &parameter, Client &currClient);
        void minuskModeParam(Channel &currChannel, Client &currClient);

        void pluslModeParam(Channel &currChannel, const std::string &parameter, Client &currClient);
        void minuslModeParam(Channel &currChannel, Client &currClient);

        void tModeParam(Channel &currChannel, const std::string &mode, Client &currClient);

        void iModeParam(Channel &currChannel, const std::string &mode, Client &currClient);

        void broadcastModeChange(Client &currClient, Channel &currChannel, const std::string &mode, std::string parameter);
        void oModeParam(Channel &currChannel, std::string &parameter, std::string &mode, Client &currClient);
        



        void sendChannelInfo(Client &currClient, Channel &currChannel);
        void channelMode(Client &currClient, std::vector<std::string> command);

        void ChannelJoin(Client &client, std::vector<std::string> command);
        void joinCommand(std::string channelName, std::string key, Client &client);

        bool Nick_isDuplicated(std::string& Nickname);
        void print_clients();

};

std::vector<std::string> parseModes(const std::string &modes);
std::vector<std::string> parseParametres(std::vector<std::string> command, std::vector<std::string> modes);
bool allParamEmpty(std::vector<std::string> parameters, std::vector<std::string> modes);

int ft_atoi(std::string parameter);
bool isValidNumber(const std::string &number);







#endif