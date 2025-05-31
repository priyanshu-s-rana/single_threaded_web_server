#include "response.h"

Networking::response::response() {
    resBodyLength = 0;
    res = "";
}

Networking::response::~response() {
    res.clear();
    resStatus.clear();
    resHeaders.clear();
    resBody.clear();
    filePath.clear();
    resBodyLength = 0;
}

const void Networking::response::setResponse(std::string method, std::string endpoint){
    if(method == "GET")  makeGetResponse(endpoint);
}

const void Networking::response::setErrorResponse(int errCode,const std::string& msg){
    errorResponse(errCode, msg);
}

//? Parsing the extension of the file asked { e.g. style.css, index.html, image.png ---- > css, html, png }
std::string Networking::response::parseExtension(const std::string& filePath) {
    // * ensuring if the endpoint have something or not.
    if(filePath.length() == 0 or filePath.find('.') == std::string::npos) return "Nothing";

    int last_dot_pos = filePath.find_last_of('.');

    if(last_dot_pos == 0 || last_dot_pos == filePath.length() - 1) return "Nothing";

    std::string extension = filePath.substr(filePath.find_last_of('.')+1);


    if (extension.size()>0 and 
        extension.find_first_of("/\\") == std::string::npos and  //* doesn't contain path separators
        UTILS::filePaths.find(extension) != UTILS::filePaths.end() and //* Ensuring we have the static filePaths for the extension provided.
        UTILS::mimeTypes.find(extension) != UTILS::mimeTypes.end())  //* Also there is a mimeType present for that extension.
        {
        return extension;
    }else{
        logError("Unsupported or Unknown extension: " + extension);
    }

    return "Nothing"; // Send NULL
}

void Networking::response::errorResponse(const int& errCode, const std::string& msg){
    if(UTILS::errorMessages.find(errCode) == UTILS::errorMessages.end()){
        logError("No Such Error Code exists: " + std::to_string(errCode));
        return;
    }
    std::string resBodyMessage = UTILS::errorMessages.at(errCode);
    resBody = "<html><body><h1>"+ resBodyMessage +"</h1></body></html>";
    resStatus = "HTTP/1.1 "+ resBodyMessage + "\r\n";
    resBodyLength = resBody.length();
    setResponseHeaders("html");
    

    logError(msg);
}

std::string Networking::response::urlDecode(const std::string& filePath) {
    if(filePath.empty()) return "";

    std::string decodedFilePath = "";
    for(int i = 0; i<filePath.length(); i++){
        if(filePath[i] == '%' and i + 2 < filePath.length()){
            std::string hex = filePath.substr(i+1,2);

            try
            {
                int charCode = std::stoi(hex, nullptr, 16);
                decodedFilePath += static_cast<char>(charCode);
                i+=2;
            }
            catch(const std::exception& e)
            {
                decodedFilePath += filePath[i];
            }
            
        }else{
            decodedFilePath += filePath[i];
        }
    }
    
    return decodedFilePath;
}

//* Extra Security Layer --> returns 'True' if the filePath is good/secured
bool Networking::response::checkFilePath(const std::string& filePath) {
    // Check if the file path is valid and the file exists
    if (filePath.find("..") != std::string::npos) {
        logError("Path traversal attempt Error: Detected '..' in the path " + filePath);
        return false;
    }

    if(!filePath.empty() and filePath[0] == '/'){
        logError("Path traversal attempt Error: getting the path starting with '/' " + filePath);
        return false;
    }
    return true;
}

bool Networking::response::readFile(const std::string& filePath, const std::string& extension) {
    log("Attempting to Serve the File at: " + filePath);

    std::ifstream read(filePath);
    if(read.is_open()){
        std::stringstream content_stream;
        content_stream << read.rdbuf();
        resBody = content_stream.str();
        
        read.close();
        
        resBodyLength = resBody.length();
        resStatus = "HTTP/1.1 200 OK\r\n";
        setResponseHeaders(extension);

        log("File Read Successfully");

        return true;
    }

    return false;
}

void Networking::response::makeGetResponse(const std::string& endpoint){
    log("GET Request received for the endpoint: "+ endpoint);
    
    if(endpoint == "/"){
        filePath = UTILS::filePaths.at("html") + "index.html";
        log("File Path: " + filePath);

        if( checkFilePath(filePath) and !readFile(filePath, "html") ){
            errorResponse(404, "No file is present at: " + filePath);
        }

        return;
    }
    
    std::string fileName = endpoint.substr(1); //*  /index.html ---> index.html, /about.html ---> about.html
    fileName = urlDecode(fileName); //* decode the fileName given if its possible.

    if(!checkFilePath(fileName)){        // * Security check step ( path should not contain '..' ))
        errorResponse(403, "Forbidden: Invalid file path");
        return;
    }
    
    std::string extension = parseExtension(fileName);

    log("File Name: " + fileName + " Extension: " + extension);
    
    if(extension != "Nothing"){      // * Checking if the extension of file asked is supported
        
        filePath = UTILS::filePaths.at(extension) + fileName;
        log("File Path: " + filePath);

        if(!readFile(filePath, extension)){
            errorResponse(500, "There is a problem while reading the file: " + fileName);
        }
    }else{
        errorResponse(404, "Unsupported file type or invalid endpoint: "+ endpoint);
    }
}

void Networking::response::makePostResponse(const std::string& endpoint) {
    // TODO :- To be implemented.
}

void Networking::response::setResponseHeaders(const std::string& fileType) {
    if(UTILS::mimeTypes.find(fileType) == UTILS::mimeTypes.end()){
        logError("No such File Type exists: " + fileType);
        errorResponse(415, "Unsupported Mime Type for the endpoint");
        return;
    }
    resHeaders = "Content-Type: " + UTILS::mimeTypes.at(fileType) + "\r\n";
    resHeaders += "Content-Length: " + std::to_string(resBodyLength) + "\r\n";
    resHeaders += "Connection: close\r\n";
}

const char* Networking::response::getResponse() {
    res = "";
    // * 1. Adding Status
    res += resStatus;
    // * 2. Adding Headers
    res += resHeaders;
    // * 3. Adding Extra Space to separate Body
    res += "\r\n";
    // * 4. Adding Body
    res += resBody;

    return res.c_str();
}

const int Networking::response::Bytes() {
    return res.length();
}
