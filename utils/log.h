#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <cerrno>
#include <cstring>



void log(const std::string& message);
void logError(const std::string& message);

#endif 