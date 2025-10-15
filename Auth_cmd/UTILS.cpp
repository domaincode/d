#include "../Client.hpp"

std::string getFromattedCurrTime()
{
    std::time_t now = std::time(nullptr);

    std::tm local_time = *std::localtime(&now);

    std::ostringstream oss;

    oss << local_time.tm_year << "-" << local_time.tm_mon << "-" << local_time.tm_mday << "  " ;
    oss << local_time.tm_hour << ":" << local_time.tm_min << ":" << local_time.tm_sec;

    return oss.str();
}


void Client::sendWelcomeMessages()
{
    std::string datetime = getFromattedCurrTime();
    std::vector<std::string> welcome_msg;
    welcome_msg.push_back("★━━━━━━━━━━━━━━━━━━ Welcome to IRC ━━━━━━━━━━━━━━━━━★");
    welcome_msg.push_back("【Connection Details】");
    welcome_msg.push_back("✦ Nickname: " + Get_nickname());
    welcome_msg.push_back("✦ Username: " + Get_username());
    welcome_msg.push_back("✦ Hostname: " + Get_hostname());
    welcome_msg.push_back("【Server Information】");
    welcome_msg.push_back("⚡ Server: " + _hostname );
    welcome_msg.push_back("⚡ Connected: " + datetime);
    for (size_t i = 0; i < welcome_msg.size(); i++)
        sendReply(": 001 " + Get_nickname() + " : " + welcome_msg[i] + "\r\n");
}

