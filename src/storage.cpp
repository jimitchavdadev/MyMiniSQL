#include "storage.hpp"
#include <fstream>
#include <filesystem>

namespace minisql {
    nlohmann::json Storage::loadTableSchema(const std::string& db_name, const std::string& table_name, const std::string& base_path) {
        std::ifstream file(base_path + "/" + db_name + "/" + table_name + "_schema.json");
        if (!file.is_open()) {
            throw std::runtime_error("Table does not exist: " + table_name);
        }
        return nlohmann::json::parse(file);
    }

    void Storage::saveTableSchema(const std::string& db_name, const std::string& table_name, const nlohmann::json& schema, const std::string& base_path) {
        std::ofstream file(base_path + "/" + db_name + "/" + table_name + "_schema.json");
        file << schema.dump(4);
    }

    nlohmann::json Storage::loadTableData(const std::string& db_name, const std::string& table_name, const std::string& base_path) {
        std::ifstream file(base_path + "/" + db_name + "/" + table_name + ".json");
        if (!file.is_open()) {
            return nlohmann::json::array();
        }
        return nlohmann::json::parse(file);
    }

    void Storage::saveTableData(const std::string& db_name, const std::string& table_name, const nlohmann::json& data, const std::string& base_path) {
        std::ofstream file(base_path + "/" + db_name + "/" + table_name + ".json");
        file << data.dump(4);
    }

} // namespace minisql