#include "Server.hpp"

#define TES 4

void test()
{
    int bytes;
        char buffer[4];
    bytes = read(0, buffer, 4);
    buffer[bytes] = 0;
    std::string str = "kamal ";



    // str.append(buffer, 4);
    // std::cout << str << std::endl;
    // std::cout << str << std::endl;
}


int main(int argc, char** argv)
{

   //test();

    if(argc != 2)
    {
        std::cout << "Error: arguments must be 2\n";
        return 1;
    }
    Server a(std::atoi(argv[1]));

   // std::cout << "FD_SETSIZE: " << FD_SETSIZE << std::endl;
    a.Start();
    a.print_clients();
    

}


