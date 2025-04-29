#pragma once
#include <string>
#include <variant>
#include <nlohmann/json.hpp>

namespace minisql {

    using json = nlohmann::json;

    // Supported data types
    enum class DataType { STRING, INT, FLOAT, BOOLEAN };

    // Data value (variant to hold any supported type)
    using DataValue = std::variant<std::string, int, float, bool>;

    // Function declarations
    std::string dataTypeToString(DataType type);
    DataType stringToDataType(const std::string& str);
    bool validateValue(const std::string& value, DataType type);
    std::string dataValueToString(const DataValue& value);
    DataValue stringToDataValue(const std::string& str, DataType type);

} // namespace minisql