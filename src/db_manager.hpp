#pragma once
#include <string>
#include <vector>
#include <map>
#include "types.hpp"
#include "parser.hpp"

namespace minisql {

    class DatabaseManager {
    public:
        DatabaseManager(const std::string& base_path);
        void executeQuery(const Query& query);
        void setCurrentDatabase(const std::string& db_name);
        std::string getCurrentDatabase() const;

    private:
        struct TableSchema {
            std::vector<TableField> fields;
            std::map<std::string, DataType> field_types;
        };

        std::string base_path_;
        std::string current_db_;
        std::map<std::string, TableSchema> tables_;

        void createDatabase(const std::string& db_name);
        void dropDatabase(const std::string& db_name);
        void createTable(const Query& query);
        void dropTable(const std::string& table_name);
        void insert(const Query& query);
        void select(const Query& query);
        void update(const Query& query);
        void deleteFrom(const Query& query);

        TableSchema loadTableSchema(const std::string& table_name);
        void saveTableSchema(const std::string& table_name, const TableSchema& schema);
    };

} // namespace minisql