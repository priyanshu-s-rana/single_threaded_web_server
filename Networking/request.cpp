#include "request.h"


Networking::request::request() {
    memset(buffer, 0, sizeof(buffer));
}

Networking::request::~request() {
    // Destructor
    // No dynamic memory allocation, so nothing to clean up
}

void Networking::request::parse() {
    // Parse the request buffer
    int i = 0;
    while(buffer[i]!= '\0'){
        while(buffer[i]=='\r' || buffer[i]=='\n'){
            i++;
        }
        if(buffer[i]=='\0'){
            break;
        }
        std::string x = "";
        while(buffer[i]!='\0' and buffer[i]!='\r'){
            x += buffer[i];
            i++;
        }
        x+='\0';
        carriages.push_back(x);
    }

    parse_method_endpoint();
}

void Networking::request::parse_method_endpoint(){
    int i = 0;
    std::string carriage = carriages[0];
    std::vector<std::string> response;

    while(carriage[i] != '\0'){
        std::string x = "";
        while(carriage[i]!='\0' and carriage[i]!=' '){
            x+=carriage[i];
            i++;
        }
        response.push_back(x);
        i++;
    }

    method = response[0];
    endpoint = response[1];
}

const std::string& Networking::request::getMethod() const {
    return method;
}
const std::string& Networking::request::getEndpoint() const {
    return endpoint;
}
