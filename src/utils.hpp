#pragma once
#include <string>
#include <vector>
#include "types.hpp"

namespace minisql {

    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    bool isValidIdentifier(const std::string& str);
    std::string toUpper(const std::string& str);

} // namespace minisql