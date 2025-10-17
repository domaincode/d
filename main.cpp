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




int main(int argc, char** argv)
{

    // std::map<int, std::string> array;

    // std::map<int, std::string>::iterator it;
    // std::map<int, std::string>::iterator tmp;

    // array[1] = "kamal";
    // array[2] = "test";
    // array[3] = "Hello world";

    // it = array.find(2);
    // tmp = it;
    // tmp++;
    // array.erase(it);


    // if(it == array.end())
    // {
    //     std::cout << "END OF MAP\n";
    // }
    // std::cout << it->second << std::endl;
    // std::cout << tmp->second  << std::endl;

    // std::cout << "start\n";

    // for(std::map<int, std::string>::iterator it = array.begin(); it != array.end(); it++)
    // {   
    //     std::cout << it->second << std::endl;
    // }

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

    return 0;
    

}


