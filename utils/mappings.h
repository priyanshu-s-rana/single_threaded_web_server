#ifndef MAPPING_H
#define MAPPING_H

#include "globals.h"
#include <unordered_map>
#include <string>

namespace UTILS {

        extern const std::unordered_map<std::string,std::string > mimeTypes;

        extern const std::unordered_map<int, std::string> errorMessages;

        extern const std::unordered_map<std::string, std::string> filePaths;
}
#endif