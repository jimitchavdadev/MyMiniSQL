#pragma once
#include <string>
#include "types.hpp"

namespace minisql {

    class Storage {
    public:
        static json loadTableSchema(const std::string& db_name, const std::string& table_name, const std::string& base_path);
        static void saveTableSchema(const std::string& db_name, const std::string& table_name, const json& schema, const std::string& base_path);
        static json loadTableData(const std::string& db_name, const std::string& table_name, const std::string& base_path);
        static void saveTableData(const std::string& db_name, const std::string& table_name, const json& data, const std::string& base_path);
    };

} // namespace minisql