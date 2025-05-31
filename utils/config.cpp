#include "config.h"


UTILS::ConfigParser::ConfigParser(const std::string& configFile){
    this->configFilePath = "config/" + configFile;
    parse();
}

UTILS::ConfigParser::~ConfigParser(){

}

void UTILS::ConfigParser::parse(){
    std::ifstream readFile(configFilePath);
    if(readFile.is_open()){
        log("Parsing the config file at: " + configFilePath);
        std::string line;
        std::string sectionHeading = "";

        while(std::getline(readFile, line)){
            if (line.empty() || (line[0]=='/' && line[1]=='/')){
                continue;
            }

            // Getting the first token.
            std::stringstream token(line);
            std::string firstChar;
            token >> firstChar;
            if(firstChar[0]=='[' and (firstChar.find(']') != std::string::npos)){
                //* Nothing to do with this section segregator
                sectionHeading = firstChar.substr(1, firstChar.find(']') - 1);
                continue;
            }

            else if(line.find('=') != std::string::npos){
                int valuePos = line.find('=') + 1;
                
                std::array<std::string, 2> values;
                //* Going until there is a character in sight.
                while ( valuePos < line.length() && (line[valuePos] == ' ' || line[valuePos] == '\t') )
                    valuePos++;
                    
                std::string key = sectionHeading.empty() ? firstChar : sectionHeading + "." + firstChar;

                //* Updating the configValues.
                if(valuePos < line.length() and line[valuePos] == '"'){
                    std::string x = "";
                    valuePos++;
                    while(valuePos < line.length() and line[valuePos] != '"'){
                        x += line[valuePos++];
                    }
                    if(x=="true" or x=="false") values[0] = "2";
                    else values[0] = "1";
                    values[1] = x;
                }else {
                    values[0] = "0";
                    values[1] = line.substr(valuePos);
                }

                configValues[key] = values;
                std::cout<<key<<"--> { "<<values[0]<<","<<values[1]<<" }\n"; 
            }
        }
        log("Parsing Completed");
        return;
    }

    logError("No such Config File exist: " + configFilePath);
}

std::string UTILS::ConfigParser::getString(const std::string& configParam,const std::string&  section) {
    std::string key = section.empty() ? configParam : section + "." + configParam;

    if(configValues.find(key) != configValues.end()){
        log("Successfully retrieved the value of param: " + key);
        std::array<std::string, 2> values = configValues.at(key);

        if(values[0] == "1"){
            return values[1];
        }else{
            logError("Incorrect data type fetching function used for the parameter");
            return "";
        }
    }
    
    logError("No such param exist in the configFile: " + configParam);
    return "";
}

long double UTILS::ConfigParser::getNumeric(const std::string& configParam,const std::string&  section) {
    std::string key = section.empty() ? configParam : section + "." + configParam;

    if(configValues.find(key) != configValues.end()){
        log("Successfully retrieved the value of param: " + key);
        std::array<std::string, 2> values = configValues.at(key);

        if(values[0] == "0"){
            try
            {
                return std::stold(values[1]);
            }
            catch(const std::exception& e)
            {
                logError("Failed to convert the value to number: " + values[1]);
                return 0.0;
            }
        }else{
            logError("Incorrect data type fetching function used for the parameter");
            return 0.0;
        }
    }
    
    logError("No such param exist in the configFile: " + configParam);
    return 0.0;
}

bool UTILS::ConfigParser::getBool(const std::string& configParam, const std::string& section){
    std::string key = section.empty() ? configParam : section + "." + configParam;

    if(configValues.find(key) != configValues.end()){
        log("Successfully retrieved the value of param: " + key);
        std::array<std::string, 2> values = configValues.at(key);

        if(values[0] == "2"){
            try
            {
                return values[1] == "true" ? true : false;
            }
            catch(const std::exception& e)
            {
                logError("Failed to convert the value to number: " + values[1]);
                return false;
            }
        }else{
            logError("Incorrect data type fetching function used for the parameter");
            return false;
        }
    }
    
    logError("No such param exist in the configFile: " + configParam);
    return false;
}

