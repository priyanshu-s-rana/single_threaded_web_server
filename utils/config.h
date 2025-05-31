#ifndef CONFIG_H
#define CONFIG_H

#include "log.h"

#include <iostream>
#include <cstring>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace UTILS{
    
    class ConfigParser{
        private:
            std::unordered_map<std::string,std::array<std::string, 2>>configValues;

            std::string configFilePath;
            std::string rootDirectory;
            std::string templateDirectory;
            std::string imageDirectory;
            std::string styleDirectory;
            void parse();
    
        public:
            ConfigParser(const std::string& configFile);
            ~ConfigParser();
            std::string getRootDirectory(); //* Gives the root directory for static files.
            std::string getString(const std::string& configParam, const std::string& section = ""); //* Get the value of a particular config parameter.
            long double getNumeric(const std::string& configParam, const std::string& section =""); //* Get the value for the parameter that we know have a numerical value
            bool getBool(const std::string& configParam, const std::string& section =""); //* Get the boolean value
    
    };
}

#endif