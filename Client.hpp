#ifndef CLIENT_HPP
#define CLIENT_HPP



#include "Server.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include "Replies_MSG.hpp"

#define BUFFER_SIZE 4
#define GREEN "\033[1;32m"
#define RESET "\033[0m"


class Server;
class Client
{
    private:
        Server* _server;
        bool is_authenticated;


        int authenticate_level;
        int _fd;
        std::string _ip;
        std::string _hostname;
        std::string _servername;
        std::string _nickname;
        std::string _username;
        std::string _fullname;

        int _authStatus;
        int _nickFlag;
        bool _registered;


        



        std::string _buffer;
        

    public:
        Client();
        ~Client();
        Client(int fd, Server* server);
        Client(const Client& other);
        Client& operator=(const Client& other);

        std::string Get_Prefix() const;
        std::string& Get_hostname();
        std::string& Get_buffer();
        std::string& Get_ip();
        int& Get_fd();
        int& get_auth_level();
        bool isFullyAuthenticated();
        std::string& Get_nickname();
        std::string& Get_username();
        std::string& Get_fullname();

        int& Get_authStatus();
        int& Get_nickFlag();

        Server& Get_serverObject();

        void Set_buffer(std::string msg);



        //Auth commands
        void sendReply(std::string response);
        void sendWelcomeMessages();
        void broadcastNickChange(const std::string &oldNick, const std::string &newNick);

        void PassCommand(std::vector<std::string> command, Server& server);
        void NickCommand(std::vector<std::string> command, Server& server);
        void UserCommand(std::vector<std::string> command);


};

std::vector<std::string> split(const std::string &str, char delimiter);

std::string trimString(const std::string &input);


#endif