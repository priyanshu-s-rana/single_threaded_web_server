#include "Networking/server.h"
#include "Networking/request.h"
#include "Networking/response.h"
#include "utils/log.h"
#include "utils/config.h"
#include "utils/globals.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cerrno>





int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    
    try
    {
        const int PORT = static_cast<int>(UTILS::config.getNumeric("port", "server"));
        Server server(PORT);
        if(!server.start()){
            logError("Failed to start the server");
            return(1);
        };

        // Server started successfully .
        
        while(true){
            std::string clientIP;
            ssize_t bytes_sent = -1;

            int clientSocketfd = server.acceptClient(clientIP);

            if(clientSocketfd<0){
                std::cerr<<"Was not able to connect with client\n";
                break;
            }

            // char buffer[4096];
            Networking::request req;
            Networking::response res;
            // memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_received = recv (clientSocketfd, req.buffer, sizeof(req.buffer)-1,0);
            if (bytes_received < 0){
                std::string msg = "Error while receiving data from client: " + clientIP;
                if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    msg = "Connection timeout for the client: " + clientIP;
                    res.setErrorResponse(408, msg);
                }
                else res.setErrorResponse(400, msg);

                bytes_sent = send(clientSocketfd, res.getResponse(), res.Bytes(), 0);
            }else if (bytes_received == 0){
                logError("Client " + clientIP + " disconnected before sending the data.");
            }else if(bytes_received == sizeof(req.buffer) - 1){
                std::string msg = "Client " + clientIP + " Request is too large";
                res.setErrorResponse(413, msg);
            }
            else {
                req.buffer[bytes_received] = '\0'; // Null-terminate the received data
                std::cout << "--------------- Client " << clientIP << " Request ---------------" << std::endl;
                std::cout << req.buffer << std::endl;
                req.parse();

                std::string method = req.getMethod();
                std::string endpoint = req.getEndpoint();
                std::cout << "Method: " << method << std::endl;
                std::cout << "Route: " << endpoint << std::endl;
                std::cout << "--------------------------------------------------------" << std::endl;

                res.setResponse(method, endpoint);

                bytes_sent = send(clientSocketfd, res.getResponse(), res.Bytes(), 0);
            }
            
            if (bytes_sent < 0) {
                logError("Error sending HTTP response to client: " + clientIP);
            } else if (bytes_sent < res.Bytes()) {
                logError("Warning: Not all of HTTP response sent to client " + clientIP);
            } else {
                log("Sent "+std::to_string(bytes_sent) + " bytes (HTTP response) to client " + clientIP);
            }

            server.closeClientSocket(clientSocketfd);
            std::cout << "Connection with client " << clientIP << " closed." << std::endl;
            std::cout << "\nServer is ready for another connection..." << std::endl;
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Server runtime error: " << e.what() << std::endl;
        return 1; // Exit with an error code
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
