#include "Server.hpp"

std::pair<int, std::string> parse_args(char **av)
{
    std::pair<int, std::string> params;

    char *end_ptr;
    long port = std::strtol(av[1], &end_ptr, 10);

    if (*end_ptr != '\0')
        throw std::invalid_argument("Port must be a numeric/integer value.");

    if (port < 1024 || port > 65535)
        throw std::invalid_argument("Port must be in the range 1–65535.");

    std::string password = av[2];
    int i = 0;
    while (password[i])
    {
        if (!isalnum(password[i]))
            throw std::invalid_argument("Password is invalid try alpha numeric characters");
        i++;
    }
    if (password.empty())
        throw std::invalid_argument("Password cannot be empty.");

    params.first = port;
    params.second = password;
    return params;
}


void ft_do(int sig)
{
    (void) sig;
     Server::EXIT_FLAG = 1;
}


int main(int argc, char** argv)
{

    // (void) argc;
    // (void)argv;

    signal(SIGINT, ft_do);
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    try
    {
        std::pair<int, std::string> params = parse_args(argv);
        Server IRC(params.first, params.second);
        IRC.Start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // std::string name;
    // std::string test;

    // name.reserve(1000);
    // //test = name;

    // std::cout << name.capacity() << std::endl;
    // std::cout << name.size() << std::endl;
    // name.clear();
    //     std::cout << name.capacity() << std::endl;
    // std::cout << name.size() << std::endl;
    //         std::cout << test.capacity() << std::endl;
    // std::cout << test.size() << std::endl;



    return 0;
    

}


