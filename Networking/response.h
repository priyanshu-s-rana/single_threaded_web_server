#ifndef RESPONSE_H
#define RESPONSE_H


#include "utils/log.h"
#include "utils/globals.h"
#include "utils/mappings.h"

#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <iostream>

namespace Networking {
    
    class response {
        private:
            std::string res;
            std::string resStatus;
            std::string resHeaders;
            std::string resBody;
            std::string filePath;
            int resBodyLength;

            // * Request Methods
            void makeGetResponse(const std::string& endpoint);
            void makePostResponse(const std::string& endpoint);

            // * Helper functions
            bool checkFilePath(const std::string& filePath);
            std::string urlDecode(const std::string& filePath);
            void errorResponse(const int& errCode, const std::string& msg);
            bool readFile(const std::string& filePath, const std::string& mimeType);
            void setResponseHeaders(const std::string& fileType);

            //* Some Parsing
            std::string parseExtension(const std::string& filePath);


        public:
            response();
            ~response();

            const char* getResponse();
            const int Bytes();
            const void setResponse(std::string method, std::string endpoint);
            const void setErrorResponse(int errCode,const std::string& msg);

    };
}

#endif
