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
    // std::vector<int>vect;

    // vect.push_back(1);
    //     vect.push_back(2);
    // vect.push_back(3);
    //     vect.push_back(44);
    //         vect.push_back(50);

    //         std::vector<int>::iterator it;
    //     std::vector<int>::iterator tmp;
    //     it = std::find(vect.begin(), vect.end(), 1);
    //     tmp = vect.end();
    //     tmp--;
    //     if(it != vect.end() && tmp != it)
    //     {
    //         std::cout << "enter to tmp != it\n";
    //         std::swap(*it, *tmp);
    //        // vect.pop_back();
    //     }

    //     for(int i = 0; i < vect.size(); i++)
    //     {
    //         std::cout << vect[i] << std::endl;
    //     }

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


