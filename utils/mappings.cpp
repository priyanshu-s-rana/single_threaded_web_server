#include "mappings.h"

namespace UTILS {
        UTILS::ConfigParser conf = UTILS::config;
        const std::unordered_map<std::string, std::string>filePaths = {
                        {"html", conf.getString("template","document")},
                        {"css",  conf.getString("style","document")},
                        {"png",  conf.getString("image","document")},
                        {"jpg",  conf.getString("image","document")},
                        {"jpeg", conf.getString("image","document")}
                };
        
        const std::unordered_map<int, std::string> errorMessages{
                        // 4xx Client Error Status Codes
                        {400, "400 Bad Request"},
                        {401, "401 Unauthorized"},
                        {402, "402 Payment Required"},
                        {403, "403 Forbidden"},
                        {404, "404 Not Found"},
                        {405, "405 Method Not Allowed"},
                        {406, "406 Not Acceptable"},
                        {407, "407 Proxy Authentication Required"},
                        {408, "408 Request Timeout"},
                        {409, "409 Conflict"},
                        {410, "410 Gone"},
                        {411, "411 Length Required"},
                        {412, "412 Precondition Failed"},
                        {413, "413 Payload Too Large"},
                        {414, "414 URI Too Long"},
                        {415, "415 Unsupported Media Type"},
                        {416, "416 Range Not Satisfiable"},
                        {417, "417 Expectation Failed"},
                        {418, "418 I'm a teapot"},
                        {421, "421 Misdirected Request"},
                        {422, "422 Unprocessable Entity"},
                        {423, "423 Locked"},
                        {424, "424 Failed Dependency"},
                        {425, "425 Too Early"},
                        {426, "426 Upgrade Required"},
                        {428, "428 Precondition Required"},
                        {429, "429 Too Many Requests"},
                        {431, "431 Request Header Fields Too Large"},
                        {451, "451 Unavailable For Legal Reasons"},
                        
                        // 5xx Server Error Status Codes
                        {500, "500 Internal Server Error"},
                        {501, "501 Not Implemented"},
                        {502, "502 Bad Gateway"},
                        {503, "503 Service Unavailable"},
                        {504, "504 Gateway Timeout"},
                        {505, "505 HTTP Version Not Supported"},
                        {506, "506 Variant Also Negotiates"},
                        {507, "507 Insufficient Storage"},
                        {508, "508 Loop Detected"},
                        {510, "510 Not Extended"},
                        {511, "511 Network Authentication Required"}
                };

        const std::unordered_map<std::string,std::string > mimeTypes  = {
                        {"html", "text/html"},
                        {"css", "text/css"},
                        {"js", "application/javascript"},
                        {"json", "application/json"},
                        {"jpg", "image/jpeg"},
                        {"png", "image/png"},
                        {"gif", "image/gif"},
                        {"txt", "text/plain"}
                };
}