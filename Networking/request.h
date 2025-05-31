#ifndef REQUEST_H
#define REQUEST_H

#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#define BUFFER_SIZE 4096

namespace Networking {
    class request {
        private:
            std::vector<std::string> carriages;
            std::string method; // Methods like GET, POST, PUT, DELETE, PATCH
            std::string endpoint;
            void parse_method_endpoint();
            
        public:
            char buffer[BUFFER_SIZE];
            request();
            ~request();
            const std::string& getMethod() const;
            const std::string& getEndpoint() const;
            void parse();
    };

}

#endif