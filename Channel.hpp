#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;

        std::vector<int> _users;
        std::vector<int> _operators;
        std::vector<int> _invited_users;



        std::string _creationDate;
        std::string _topicDate;
        std::string _topicSetter;

        size_t _userLimit;
        bool _inviteOnly;
        bool _topicLock;


        std::vector<bool> Modes;
    
    public:
        Channel();
        ~Channel();
        Channel(std::string& Channel_name, std::string& key);

        void addClient(int& client_fd);
        void addOperator(int& client_fd);
        void addInvited(int& client_fd);
        void removeOperator(int& client_fd);
        bool removeClientFromChannel(int client_fd);


        //chekers
        bool isClientInChannel(int client_fd);
        bool isOperator(int& client_fd);
        bool isInvited(int& client_fd);


        void broadcastMessage(std::string message, std::map<int, Client>& _clients);


        //geters
        std::string& Get_creationDate();
        std::string Get_ChannelModes();
        std::string& Get_name();
        std::vector<int>& Get_users();
        std::vector<int>& Get_operators();
        const std::string& Get_key()const;
        size_t Get_userCount();
        size_t& Get_userLimit();
        bool& Get_inviteOnly();
        std::string& Get_topic();
        std::string& Get_topicSetter();
        std::string& Get_topicDate();
        bool Get_topicLock()const;
        std::string getAllUsersNames(std::map<int, Client>& _clients);

        //seters
        void Set_inviteOnly(bool status);
        void Set_topicLock(bool status);
        void Set_key(const std::string& key);

};

std::string getCurrTime(void);





#endif