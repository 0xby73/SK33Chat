#include "netutils.h" // Networking libraries

#include "utils.h" // Functions like Clear_Screen
#include "uuid.h" // Client ID generator

#include "colors.h" // Colors
#include "help.h" // Help messages | Command list
#include "ascii.h"

#include "declare.h" // Declaring functions
#include "prototypes.h" // Function prototypes | WIP Functions

#include "client_events.h" // Client events. eg, Join/Leave

#include <iostream>
#include <cstring>

#include <vector>
#include <map>

#include <thread>
#include <mutex>

#include <fstream>

#include <algorithm>

#include <chrono>
#include <atomic>

struct ClientStruct
{
  int SOCK;
  std::string USERNAME;
  std::string UUID;
};

std::map<std::string, ClientStruct> clientmap;

std::atomic<bool> Run(true);
bool Show_Menu = true;

std::vector<int> clients;
std::mutex clientmutex;
std::mutex clientmutex2;

unsigned short port = 8080; // Set by default
std::string TargetUser = "Blank";

static void Broadcast_Message(const std::string& msg, const int clientsocket) noexcept {
    std::lock_guard<std::mutex> lock(clientmutex);
    for (const auto& client : clients)
    {
        if (client != clientsocket)
        {
            if (send(client, msg.c_str(), msg.size(), 0) == -1)
            {
                std::cerr << "Error sending to client " << client << " Error: " << strerror(errno) << '\n';
            }
        }
    }
}



// Server to client Interactions Begin - Events will be in the Header file, eventually
static void Execute_Ban(const int clientsocket) noexcept
{
    if (TargetUser != "Blank")
    {
        std::cout << "Attempting to ban '" << TargetUser << "' \n";

        std::lock_guard<std::mutex> lock(clientmutex);
        auto it = clientmap.find(TargetUser);
        if (it != clientmap.end()) {
            close(it->second.SOCK); 
            clientmap.erase(it);
            std::cout << RED << TargetUser << " has been kicked\n";
        }
        else
        {
            std::cout << "Couldn't ban user\n";
        }
    }
    else
    {
        std::cout << "TargetUser is blank\n";
    }
    TargetUser = "Blank";
}

static void Server_Send(const int clientsocket) noexcept
{
    std::cout << "Command execution enabled.\n"
        "type !help for a command list\n";
    while(Run)
    {
        std::string servermsg;
        std::getline(std::cin, servermsg);

        if (servermsg.empty())
        {
            std::cout << "No command executed\n";
            continue;
        }
        
        if (servermsg.rfind("!ban", 0) == 0)
            // but now i'm getting another erroor here
        {
            TargetUser = servermsg.substr(4);
            TargetUser.erase(std::remove_if(TargetUser.begin(), TargetUser.end(), ::isspace), TargetUser.end());
            Execute_Ban(clientsocket);

        }
        else
        {
            std::cout << "Command not found\n";
        }


    }
}

// Handle Client Begin
static void Client_Handle(const int clientsocket) noexcept
{
    Client_Assign(clientsocket);

    Client_Join(clientsocket);

    // While true looped function
    Receive_Messages(clientsocket);
}

static void Client_Assign(const int clientsocket) noexcept
{
    std::string uuid = Uuid::UUID_Gen();
   
    {
    std::lock_guard<std::mutex> lock(clientmutex);
    clients.push_back(clientsocket);
    }

    // Client_Join() Addon
    std::cout << "-----------------------------------\n";
    std::cout << GREEN << "New client connected: " << RESET << clients.size() << '\n';
    std::cout << BLUE << "Username: ";
    
    std::string Username = Username_Receive(clientsocket);

    ClientStruct client;
    client.SOCK = clientsocket;
    client.USERNAME = Username;
    client.UUID = uuid;

    clientmap[Username] = client;

    // This is also a part of Client_Join() for now
    std::cout << RESET << "UUID - "<< client.UUID << "---- Useless for now\n";
    // std::cout << client.USERNAME << '\n';
    std::cout << "Socket Index - " << client.SOCK << '\n';
}

static std::string Username_Receive(const int clientsocket) noexcept
{   
    std::string Username = "Unnamed";

    char buffer[1024] = {0};
    const int recvbytes = recv(clientsocket, buffer, sizeof(buffer), 0);
    if (recvbytes < 0)
    {
        std::cout << "Failed to retrive username." << '\n';
    }
    else
    {
        Username = Client_Username(buffer);
        
    }
    return Username;

}

static std::string Client_Username(std::string message) noexcept
{

    const int length = message.length();

    if (message.find('不'))
    {
        std::cout << message.substr(0, length-1) << '\n';

    }
    else
    {
        message = "BLANK";
    }

    return message;
}


static void Client_Join(const int clientsocket) noexcept
{

    /*
     Will change up how this is formatted, messy ATM
     I will clean it up after i implement something like
     curses, etc.
     */
    std::cout << "-----------------------------------\n";
    for(;;)
    {
    Receive_Messages(clientsocket);
    }
}

static void Client_Leave(const int clientsocket) noexcept
{
    std::lock_guard<std::mutex> lock(clientmutex);
    clients.erase(std::remove(clients.begin(), clients.end(), clientsocket), clients.end());
    std::cout << "-----------------------------------\n";
    std::cout << RED << "Client disconnected | clients: " << clients.size() << RESET << '\n'; 
    std::cout << "-----------------------------------\n";
    close(clientsocket);
}

static void Receive_Messages(const int clientsocket) noexcept
{
    while(true)
    {
        char buffer[1024] = {0};
        const int recvbytes = recv(clientsocket, buffer, sizeof(buffer), 0);
        if (recvbytes < 0)
        {
            std::cerr << "Failed to receive message." << strerror(errno) << '\n';
            break;
        }
    
        else if (recvbytes == 0)
        {
            break;
        }
    
        else
        {
            std::string MESSAGE(buffer, recvbytes);
        
            Broadcast_Message(MESSAGE, clientsocket);
            std::cout << MESSAGE << '\n';

        }
        memset(buffer, 0, sizeof(buffer));
    }

    // THIS SHIT IS BROKEN IT SPAMS IT WHEN YOU LEAVE, WILL FIX L8R
    Client_Leave(clientsocket);
    
}

/*
static void Client_Display() noexcept
{
    std::cout << "| User ID ---|--- Socket Number ---|\n"
                 "--------------------------------------\n";

    ClientStruct client;
    std::lock_guard<std::mutex> lock(clientmutex);
    for (const auto& client : clients) 
    {
        std::cout << client.UUID << " | " << client.SOCK << " | \n";
    }
}
*/

// Handle Client End

// Networking Begin --------------------

// 0
static int Create_Socket() noexcept
{
	int serversocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (serversocket == -1)
	{
		std::cerr << "Failed to create socket\n";
		return -1;
	}
	return serversocket;
}

// 1
static bool Bind_Socket(const int serversocket) noexcept
{
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));

  saddr.sin_family = AF_UNSPEC; //AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serversocket, reinterpret_cast<struct sockaddr*>(&saddr), sizeof(saddr)) == -1)
	{
		std::cerr << "Error binding socket\n";
		close(serversocket);
		
		return false;
	}
	return true;
}

// 2
static bool Listen_Socket(const int serversocket) noexcept
{
	if (listen(serversocket, 7) == -1)
	{
		std::cerr << "Error listening on socket\n";
		close(serversocket);
		return false;
	}
	return true;
}

// 3
static int Accept_Socket(const int serversocket) noexcept
{
	int clientsocket = accept(serversocket, NULL, NULL);

	if (clientsocket == -1) 
	{
    std::cerr << "Error accepting client connection\n";
    return -1;
  }
	return clientsocket;
}
// Networking End --------------------
// Ugly section, will  clean it up && move it somewhere "cleaner"
static void Select_Options() noexcept
{
    while(Show_Menu)
    {
        int options;
        Utils::Clear_Screen();

        std::cout << "Hosting IP Address: **Selected by default**\n"; 
        std::cout << "1. Port " << port << '\n';
        std::cout << "-------------------------\n"
                     "0. Start\n"
                     "# >> ";
        std::cin >> options;
        switch(options)
        {
            case 0:
                Show_Menu = false;
                break;
            case 1:
                Utils::Clear_Screen();
                int select;
                std::cout << "Selected Port: " << port << '\n';
                std::cout << "Select a port number '0 - 65,535'\n";
                std::cout << "0. Go back\n";
                std::cout << "---------------------------------\n";
                std::cout << "# >> ";
                std::cin >> select;
                if (select == 0)
                {
                    Utils::Clear_Screen();
                    break;
                }
                else if (select > 65535)
                {
                    std::cout << "\nInvalid port: " << select << "Max port: 65535\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
                else
                {
                    port = select;
                    std::cout << "Port selected: " << port << '\n';
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    break;
                }
            default:
                std::cout << "Please pick a valid option\n";
                std::cout << "You picked: " << select;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
                



                


                
        }
    }

}

int main()noexcept
{
    Select_Options();
	
	// Functions Begin

	Utils::Clear_Screen();

	std::cout << "Triptych / 0xby73 - [*] Listener started > " << port << '\n';
    

	// Functions End
	int serversocket = Create_Socket();
	if (serversocket == -1)
	{
		return 1;
	}

	if (!Bind_Socket(serversocket))
	{
		return 1;
	}

	if (!Listen_Socket(serversocket))
	{
		return 1;
	}

    int runonce = 0;
	for (;;)
	{
  	int clientsocket = Accept_Socket(serversocket);
	 if (clientsocket == -1)
	 {
          continue;

	 }

		std::thread fortnite(Client_Handle, clientsocket);
        fortnite.detach();
      
        if (runonce < 1)
        {
          std::cout << runonce << '\n';
          std::thread bigchungus(Server_Send, clientsocket);
          bigchungus.detach();
          ++runonce;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}

}
