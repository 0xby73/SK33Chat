#include "netutils.h" // Networking libraries

#include "utils.h" // Functions like Clear_Screen
#include "uuid.h" // Client ID generator

#include "colors.h" // Colors
#include "help.h" // Help messages | Command list

#include "declare.h" // Declaring functions
#include "prototypes.h" // Function prototypes | WIP Functions
#include "ascii.h"

#include "client_events.h" // Client events. eg, Join/Leave

#include <iostream>
#include <cstring>

#include <vector>
#include <map>

#include <thread>
#include <mutex>

#include <fstream>

#include <algorithm>
#include <atomic>

#include <errno.h>

std::mutex mtx;
std::atomic<bool> Run(true);


// Receive and Send Begin ---------------
static void Receive_Message(const int clientsocket) noexcept
{
    char buffer[1024] = {0};
    while(Run)
    {
        memset(buffer, 0, sizeof(buffer));
        const int recvbytes = recv(clientsocket, buffer, sizeof(buffer), 0);
        if (recvbytes > 0)
        {
            buffer[recvbytes] = '\0';
            
            std::string MESSAGE(buffer, recvbytes);
            std::cout << Utils::Get_Time() << " | " << MESSAGE << '\n';
        }
        else if (recvbytes == 0)
        {
            std::cout << "Server has shutdown.\n";
            Run = false;
            break;
        }
        else
        {
            std::cerr << "Error in recv(). Error code: " << errno << ", Error message: " << strerror(errno) << std::endl;
            Run = false;
            break;
        }
    }
    return;
}

static void Send_Message(const int clientsocket, const std::string& Username) noexcept
{
    while(Run)
    {
        std::string msg;
        std::cout << Utils::Get_Time() << " | "<< Username + ": ";
        std::getline(std::cin, msg);

        if (msg.empty())
        {
            continue;
        }

        msg = Username + ": " + msg;

        send(clientsocket, msg.c_str(), msg.length(), 0);

    }
}

// Receive and Send End ---------------

static std::string Username_Picker()
{
    std::string Username;
    std::cout << "Enter a Username: "; 
    std::cin >> Username;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return Username;
}

static void Send_Username(const int clientsocket, const std::string& username) noexcept
{
    std::string msg = username + ' ';
    send(clientsocket, msg.c_str(), msg.length(), 0);
}

// Networking Begin -----------------------

// 0
static int Create_Socket() noexcept
{
        int clientsocket = socket(AF_INET, SOCK_STREAM, 0);

        if (clientsocket == -1)
        {
                std::cerr << "Failed to create socket\n";
                return -1;
        }
        return clientsocket;
}

// 1
static bool Connect_Socket(const std::string& IP, const int port, const int clientsocket) noexcept
{
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));

    saddr.sin_family = /* AF_UNSPEC; */ AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
  
    if (inet_pton(AF_INET, IP.c_str(), &saddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid IP Address\n";
        return false;
    }
     
    if (connect(clientsocket, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        std::cerr << "Connection failed\n";
        return false;
    }

    return true;
}


int main(const int argc, char **argv) noexcept
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " IP " << "PORT\n" << RESET;
        return 1;
    }

    std::string IP = argv[1];
    int Port = std::stoi(argv[2]);

    // Functions Begin
    Utils::Clear_Screen();
    Ascii::Header_Client();
    
    std::string Username = Username_Picker();
    // Functions End

    int clientsocket = Create_Socket();
    if (clientsocket == -1)
    {
        return 1;
    }

    
    if (!Connect_Socket(IP, Port, clientsocket))
    {
            return 1;
    }

    Send_Username(clientsocket, Username);

    std::thread Receive_Thread(Receive_Message, clientsocket);
    std::thread Send_Thread(Send_Message, clientsocket, Username);

    Receive_Thread.join();
    Send_Thread.join();

}
