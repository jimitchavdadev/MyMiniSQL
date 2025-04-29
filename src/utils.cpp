#include "utils.hpp"
#include <algorithm>
#include <cctype>

namespace minisql {

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        for (char c : str) {
            if (c == delimiter) {
                if (!token.empty()) {
                    tokens.push_back(trim(token));
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }

    std::string trim(const std::string& str) {
        size_t start = 0, end = str.size();
        while (start < end && std::isspace(str[start])) ++start;
        while (end > start && std::isspace(str[end - 1])) --end;
        return str.substr(start, end - start);
    }

    bool isValidIdentifier(const std::string& str) {
        if (str.empty() || !std::isalpha(str[0])) return false;
        return std::all_of(str.begin(), str.end(), [](char c) {
            return std::isalnum(c) || c == '_';
        });
    }

    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

} // namespace minisql