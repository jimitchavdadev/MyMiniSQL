#include "types.hpp"
#include "utils.hpp"

namespace minisql {

std::string dataTypeToString(DataType type) {
    switch (type) {
        case DataType::STRING: return "STRING";
        case DataType::INT: return "INT";
        case DataType::FLOAT: return "FLOAT";
        case DataType::BOOLEAN: return "BOOLEAN";
        default: return "UNKNOWN";
    }
}

DataType stringToDataType(const std::string& str) {
    std::string upper = toUpper(str);
    if (upper == "STRING") return DataType::STRING;
    if (upper == "INT") return DataType::INT;
    if (upper == "FLOAT") return DataType::FLOAT;
    if (upper == "BOOLEAN") return DataType::BOOLEAN;
    throw std::runtime_error("Invalid data type: " + str);
}

bool validateValue(const std::string& value, DataType type) {
    try {
        switch (type) {
            case DataType::STRING:
                return !value.empty();
            case DataType::INT:
                std::stoi(value);
                return true;
            case DataType::FLOAT:
                std::stof(value);
                return true;
            case DataType::BOOLEAN: {
                auto upper = toUpper(value);
                return upper == "TRUE" || upper == "FALSE";
            }
            default:
                return false;
        }
    } catch (...) {
        return false;
    }
}

std::string dataValueToString(const DataValue& value) {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::string>) return v;
        else if constexpr (std::is_same_v<T, bool>) return v ? "true" : "false";
        else return std::to_string(v);
    }, value);
}

DataValue stringToDataValue(const std::string& str, DataType type) {
    switch (type) {
        case DataType::STRING:
            return str;
        case DataType::INT:
            return std::stoi(str);
        case DataType::FLOAT:
            return std::stof(str);
        case DataType::BOOLEAN:
            return toUpper(str) == "TRUE";
        default:
            throw std::runtime_error("Invalid data type for value: " + str);
    }
}

} // namespace minisql