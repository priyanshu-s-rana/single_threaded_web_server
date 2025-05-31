#include "server.h"

// Constructor of Server
Server::Server (int port) : port (port), listenSocketFd (-1) {
    log ("Initializing server on port " + std::to_string (port));

    // Creating a TCP socket
    listenSocketFd = socket (AF_INET, SOCK_STREAM, 0); 
    if (listenSocketFd < 0) {
        throw std::runtime_error ("Failed to create socket" + std::string(strerror(errno))); 
    }

    log ("Socket created on port " + std::to_string (port) + " with file descriptor " + std::to_string (listenSocketFd) + " successfully");

    // Allow address reuse
    //* So that server can bind at same port even if they are in 'TIME_WAIT' state, after stopping the server.
    int opt = UTILS::config.getBool("reuse_address", "development"); // Tells the system to enable the option
    if( (setsockopt (listenSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt))) < 0 ){
        logError ("Server Warning : Failed to set SO_REUSEADDR option" + std::string(strerror(errno)));
    }else log ("SO_REUSEADDR option set successfully");

    // Setting up the server address structure
    memset (&serverAddr, 0, sizeof(serverAddr));
    //? sin :- socket internet or socket inet [Indicating these fields are for IP socket address]
    serverAddr.sin_family = AF_INET; // IPv4
    //! expects htonl() to convert the port number from host byte order to network byte order {32 bit addresses}
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 {Bind to any address}
    //! htons() converts the port number from host byte order to network byte order
    serverAddr.sin_port = htons (port); // Port number

    memset (serverAddr.sin_zero, 0, sizeof (serverAddr.sin_zero)); // Zero out the rest of the structure

    log ("Server initialized successfully");
}

// Destructor of Server.
Server::~Server () {
    log ("Shutting down server");
    if (listenSocketFd >= 0) {
        if ((close (listenSocketFd))< 0) {
            logError ("Server Warning : Failed to close socket " + std::to_string (listenSocketFd) + " : " + std::string(strerror(errno)));
        }
        listenSocketFd = -1;
    }
    log ("Server shutdown complete");
}

bool Server::start () {
    if (listenSocketFd < 0) {
        logError ("Server Error: Socket not created.");
        return false;
    }
    // Binding the socket to the address
    if ( bind (listenSocketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        logError ("Server Error: Failed to bind socket to address ");
        return false;
    }
    log ("Socket bound to address successfully");

    // listening to the server
    log ("Listening to connection ...");
    if ( listen (listenSocketFd, 10) < 0 ) {
        logError("Server Error: Failed to listen on socket " + std::string(strerror(errno)));
        return false;
    }

    log ("Server is listening on port " + std::to_string (port));
    return true;
}

/*
    args:-
    clientIP : [string] local variable passed by reference to get updated by client IP address.

    return:-
    clientSocketfd : [int] client socket file descriptor.
*/
int Server::acceptClient (std::string &clientIP) {
    if( listenSocketFd < 0){
        logError("Server Error: cannot accept client Server socket is not created.");
        return -1;
    }

    struct sockaddr_in clientAddr; // To store client Address information.
    socklen_t clientAddrLen = sizeof(clientAddr); // Size of client address structure.
    log("Waiting to accept new connection ...");
    int clientSockfd = accept( this->listenSocketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if(clientSockfd < 0) {
        logError("Server Error: unable to create client Socket.");
        return -1;
    }else{
        long double timeout = UTILS::config.getNumeric("rcv_timeout", "server") ;
        struct timeval tv;
        tv.tv_sec = (time_t)timeout; //* Whole second
        tv.tv_usec = (suseconds_t)((timeout - tv.tv_sec) * 1e6); //* Fractional part.

        if(timeout == 0.0 or  setsockopt(clientSockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0){
            logError ("Server Warning : Failed to set SO_RCVTIMEO option");
            return -1;
        }else{
            log("SO_RCVTIMEO set to " + std::to_string(timeout) + " seconds for client socket fd: " + std::to_string(clientSockfd));
        }
    }

    char client_ip_addr[INET_ADDRSTRLEN];
    /*
    * inet_ntop() :- Convert an IPv4 or IPv6 address from its binary form (in network byte order) to a string in the standard text format.{ntop - network to presentation}
    * INET_ADDRSTRLEN :- Size of the buffer to hold the string representation of the address.
    */
    if(inet_ntop(AF_INET, &clientAddr.sin_addr, client_ip_addr, INET_ADDRSTRLEN ) != NULL){
        clientIP = client_ip_addr;
        log("Accepted the connection with ip address " + clientIP + ":" + std::to_string(clientAddr.sin_port) + "with client socket fd : " + std::to_string(clientSockfd));
    }else {
        logError("Server Warning: inet_ntop failed to convert client IP");
        clientIP = "unknown";
    }

    return clientSockfd;
}

/*
    args:-
    sockfd : [int]  client socket file descriptor.
*/

void Server::closeClientSocket (int sockfd) {
    if(sockfd >=0) {
        if (close(sockfd) < 0) {
            logError("Server Error : Failed to close the client ");
        }else{
            log("Closing client socket fd: "+std::to_string(sockfd));
        }
    }
}