#include "Server.hpp"

void Server::launchBOT()
{
    int bot_fd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);

    if (bot_fd < 0)
    {
        return;
    }

    if (connect(bot_fd, (sockaddr *)&sAddress, sizeof(sAddress)) < 0)
    {
        close(bot_fd);
        return;
    }

    Client botClient(bot_fd, this);

    botClient.Get_nickname() = "SECBOT";
    botClient.Get_username() = "SECBOT";
    botClient.Get_ip() = _hostname;
    botClient.Get_fullname() = "SECBOT";
    botClient.Get_authStatus() = 0x07;

    // _clients[bot_fd] = botClient;
    // pollfd fd;
    // memset(&fd, 0, sizeof(fd));
    // fd.events = POLLIN;
    // fd.fd = bot_fd;
    // _fds.push_back(fd);

}

void webPath(Client &client,std::string server_hostname)
{
    std::string sender = "SECBOT";

    const char *messages[] = {
        "✦ SECBOT Web Application Security Learning Path ✦",
        "",
        "【Core Learning Platforms】",
        "⚡ PortSwigger Web Security Academy",
        "   • URL: https://portswigger.net/web-security",
        "   • Comprehensive, free labs and materials",
        "   • Server-side: SQLi, XXE, SSRF, Path Traversal",
        "   • Client-side: XSS, CSRF, CORS, Clickjacking",
        "   • Advanced: JWT, OAuth, GraphQL, Prototype Pollution",
        "",
        "⚡ HackTheBox Academy",
        "   • URL: https://academy.hackthebox.com",
        "   • Structured modules with hands-on labs",
        "   • Web fundamentals to advanced exploitation",
        "",
        "⚡ PentesterLab",
        "   • URL: https://pentesterlab.com",
        "   • Progressive learning badges",
        "   • Essentials → Unix → PCAP → White Badge",
        "   • Real-world vulnerability scenarios",
        "Reminder: Always follow platform rules and practice ethically.",
        "Tip: Take detailed notes and document your learning journey!",
        ">>> TYPE 'BACK' TO RETURN TO THE PREVIOUS PAGE. <<< "};

    size_t numMessages = sizeof(messages) / sizeof(messages[0]);

    for (size_t i = 0; i < numMessages; ++i)
    {
        std::string line = messages[i];
        std::string formatted_msg = PRIVMSG_FORMAT(sender, sender, server_hostname, client.Get_nickname(), line + "\r\n");
        client.sendReply(formatted_msg);
    }
}

void forPath(Client &client,std::string server_hostname)
{
    std::string sender = "SECBOT";

    const char *messages[] = {
        "✦ SECBOT Digital Forensics & Incident Response Path ✦",
        "",
        "【Core Learning Platforms】",
        "⚡ SANS Digital Forensics",
        "   • URL: https://www.sans.org/cyber-security-courses/digital-forensics-essentials",
        "   • Industry standard training",
        "   • Hands-on forensics investigation",
        "   • Incident response procedures",
        "",
        "【Recommended Books】",
        "• The Shellcoder's Handbook",
        "• Practical Malware Analysis",
        "• Hacking: The Art of Exploitation",
        "",
        "Tip: Build a dedicated Linux VM for practice!",
        "Reminder: Always practice ethically and legally.",
        ">>> TYPE 'BACK' TO RETURN TO THE PREVIOUS PAGE. <<< "

    };

    size_t numMessages = sizeof(messages) / sizeof(messages[0]);

    for (size_t i = 0; i < numMessages; ++i)
    {
        std::string line = messages[i];
        std::string formatted_msg = PRIVMSG_FORMAT(sender, sender, server_hostname, client.Get_nickname(), line + "\r\n");
        client.sendReply(formatted_msg);
    }
}

void windowsPentestPath(Client &client, std::string server_hostname)
{
    std::string sender = "SECBOT";
    const char *messages[] = {
        "✦ SECBOT Windows Penetration Testing Learning Path ✦",
        "",
        "【Core Learning Platforms】",
        "⚡ Offensive Security WinEXP",
        "   • URL: https://www.offensive-security.com/winexd/",
        "   • Comprehensive Windows exploitation training",
        "   • Active Directory, privilege escalation",
        "   • Hands-on lab environments",
        "",
        "Reminder: Always practice in controlled environments.",
        "Tip: Build a home lab for safe practice!",
        ">>> TYPE 'BACK' TO RETURN TO THE PREVIOUS PAGE. <<< "};
    size_t numMessages = sizeof(messages) / sizeof(messages[0]);
    for (size_t i = 0; i < numMessages; ++i)
    {
        std::string line = messages[i];
        std::string formatted_msg = PRIVMSG_FORMAT(sender, sender, server_hostname, client.Get_nickname(), line + "\r\n");
        client.sendReply(formatted_msg);
    }
}
void defaultAnswer(Client &client,std::string server_hostname)
{
    std::string sender = "SECBOT";
    const char *messages[] = {
        "✦ Welcome to SECBOT - Your Security Learning Assistant! ✦",
        "【Available Learning Paths】",
        "",
        "⚡ FOR - Digital Forensics & Incident Response",
        "",
        "⚡ WEB - Web Application Security",
        "",
        "⚡ WIN - Windows Systems Penetration Testing",
        "",
        "Usage: SECBOT <path>",
        "Examples:",
        "  SECBOT FOR",
        "  SECBOT WIN",
        "",
        "Tip: Each path includes curated resources, practice labs, and recommended certifications.",
        "Type your chosen path to begin your cybersecurity journey!"};
    size_t numMessages = sizeof(messages) / sizeof(messages[0]);
    for (size_t i = 0; i < numMessages; ++i)
    {
        std::string line = messages[i];
        std::string formatted_msg = PRIVMSG_FORMAT(sender, sender, server_hostname, client.Get_nickname(), line + "\r\n");
        client.sendReply(formatted_msg);
    }
}

void Server::BotCommand(int client_fd, std::vector<std::string> command)
{
    Client &currClient = _clients[client_fd];
    std::string sender = "SECBOT";
    if (command.size() < 2)
    {
        const char *messages[] = {
            "✦ Welcome to SECBOT - Your Security Learning Assistant! ✦",
            "【Available Learning Paths】",
            "",
            "⚡ FOR - Digital Forensics & Incident Response",
            "   Investigate security incidents, perform malware analysis, and master evidence collection",
            "   Skills: Memory forensics, Network forensics, Malware analysis, Chain of custody",
            "",
            "⚡ WEB - Web Application Security",
            "   Learn to identify, exploit, and secure web applications against common threats",
            "   Skills: OWASP Top 10, XSS, SQLi, Authentication bypasses, API security",
            "",
            "⚡ WIN - Windows Systems Penetration Testing",
            "   Active Directory, PowerShell, Windows privileges, Service exploits",
            "",
            "Usage: SECBOT <path>",
            "Examples:",
            "  SECBOT FOR - Shows forensics learning resources and roadmap",
            "  SECBOT WIN - Shows Windows pentesting resources and methodology",
            "",
            "Tip: Each path includes curated resources, practice labs, and recommended certifications.",
            "Type your chosen path to begin your cybersecurity journey!"};

        size_t numMessages = sizeof(messages) / sizeof(messages[0]);
        for (size_t i = 0; i < numMessages; ++i)
        {
            std::string line = messages[i];
            std::string formatted_msg = PRIVMSG_FORMAT(sender, sender, _hostname, currClient.Get_nickname(), line + "\r\n");
            currClient.sendReply(formatted_msg);
        }
        return;
    }
    else if (command[1] == "WEB")
        webPath(currClient, _hostname);
    else if (command[1] == "FOR")
        forPath(currClient, _hostname);
    else if (command[1] == "WIN")
        windowsPentestPath(currClient, _hostname);
    else
        defaultAnswer(currClient, _hostname);
}
