#ifndef SERVER_H
#define SERVER_H

#include "utils/log.h"
#include "utils/globals.h"

#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <cstring>
#include <iostream>
#include <string>

class Server {
    public: 
        Server(int port);
        ~Server();
        bool start();
        int acceptClient(std::string &clientIP);
        void closeClientSocket(int sockfd);
    
    private:
        int port; //* Port number for the server
        int listenSocketFd;   //* Listening socket file descriptor
        struct sockaddr_in serverAddr; //* Server address structure

        //! Disable copy and move operations
        Server(const Server&) = delete; // Disable copy constructor
        Server& operator=(const Server&) = delete; // Disable copy assignment operator
        Server(Server&&) = delete; // Disable move constructor
        Server& operator=(Server&&) = delete; // Disable move assignment operator
};

#endif //* end the conditional block started by #ifndef