#pragma once
#include <string>
#include <vector>
#include "types.hpp"

namespace minisql {

    enum class QueryType {
        CREATE_DATABASE,
        DROP_DATABASE,
        USE_DATABASE,
        CREATE_TABLE,
        DROP_TABLE,
        INSERT,
        SELECT,
        UPDATE,
        DELETE,
        UNKNOWN
    };

    struct TableField {
        std::string name;
        DataType type;
    };

    struct Query {
        QueryType type;
        std::string database;
        std::string table;
        std::vector<TableField> fields; // For CREATE TABLE
        std::vector<std::string> select_fields; // For SELECT
        std::vector<std::pair<std::string, std::string>> insert_values; // For INSERT
        std::vector<std::pair<std::string, std::string>> update_values; // For UPDATE
        std::string where_field;
        std::string where_value;
    };

    class Parser {
    public:
        Query parse(const std::string& query_str);

    private:
        bool validateQuerySyntax(const Query& query);
    };

} // namespace minisql