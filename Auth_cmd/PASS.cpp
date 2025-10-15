#include "../Client.hpp"


void Client::PassCommand(std::vector<std::string> command, Server& server)
{
    //std::cout << GREEN << "Enter To pass Command\n" << RESET;
    if (command.size() < 2)
    {
        sendReply(ERR_NEEDMOREPARAMS(Get_nickname(), Get_hostname(), "PASS"));
        return;
    }

    std::string password = command[1];
    if (server.Get_password() == password && Get_authStatus() == 0x07)
    {
        sendReply(ERR_ALREADYREGISTERED(Get_nickname(), Get_hostname()));
        return;
    }

    if (server.Get_password() == password)
    {
        if (Get_authStatus() < 0x01)
        {
            Get_authStatus() |= 0x01;
            // currClient.setAuthStatus();
        }
    }
    else
    {
        sendReply(ERR_PASSWDMISMATCH(Get_nickname(), Get_hostname()));
    }
}