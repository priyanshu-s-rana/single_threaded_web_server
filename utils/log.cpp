#include "log.h"

void log (const std::string& message) {
    std::cout <<"[LOG]: " << message << std::endl;
}

void logError (const std::string& message) {
    std::cerr << "[ERROR]: "<<strerror(errno)<<" "<<message<<std::endl;
}