#include "parser.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <regex>

namespace minisql {

Query Parser::parse(const std::string& query_str) {
    Query query;
    std::string cleaned = trim(query_str);
    if (cleaned.empty() || cleaned.back() != ';') {
        throw std::runtime_error("Query must end with semicolon");
    }
    cleaned = cleaned.substr(0, cleaned.size() - 1); // Remove semicolon
    std::vector<std::string> tokens = split(cleaned, ' ');
    if (tokens.empty()) {
        throw std::runtime_error("Empty query");
    }

    std::string cmd = toUpper(tokens[0]);

    if (cmd == "CREATE") {
        if (tokens.size() < 3) throw std::runtime_error("Invalid CREATE syntax");
        if (toUpper(tokens[1]) == "DATABASE") {
            query.type = QueryType::CREATE_DATABASE;
            query.database = tokens[2];
            if (!isValidIdentifier(query.database)) {
                throw std::runtime_error("Invalid database name");
            }
        } else if (toUpper(tokens[1]) == "TABLE") {
            query.type = QueryType::CREATE_TABLE;
            std::string rest = cleaned.substr(cleaned.find("TABLE") + 5);
            std::regex table_regex(R"(\s*(\w+)\s*\((.*)\))");
            std::smatch match;
            if (!std::regex_match(rest, match, table_regex)) {
                throw std::runtime_error("Invalid CREATE TABLE syntax");
            }
            query.table = match[1];
            if (!isValidIdentifier(query.table)) {
                throw std::runtime_error("Invalid table name");
            }
            std::string fields_str = match[2];
            std::vector<std::string> field_pairs = split(fields_str, ',');
            for (auto& pair : field_pairs) {
                auto parts = split(trim(pair), ' ');
                if (parts.size() != 2) {
                    throw std::runtime_error("Invalid field definition");
                }
                TableField field;
                field.name = parts[0];
                field.type = stringToDataType(parts[1]);
                if (!isValidIdentifier(field.name)) {
                    throw std::runtime_error("Invalid field name: " + field.name);
                }
                query.fields.push_back(field);
            }
        } else {
            throw std::runtime_error("Invalid CREATE command");
        }
    } else if (cmd == "DROP") {
        if (tokens.size() != 3) throw std::runtime_error("Invalid DROP syntax");
        if (toUpper(tokens[1]) == "DATABASE") {
            query.type = QueryType::DROP_DATABASE;
            query.database = tokens[2];
        } else if (toUpper(tokens[1]) == "TABLE") {
            query.type = QueryType::DROP_TABLE;
            query.table = tokens[2];
        } else {
            throw std::runtime_error("Invalid DROP command");
        }
    } else if (cmd == "USE") {
        if (tokens.size() != 2) throw std::runtime_error("Invalid USE syntax");
        query.type = QueryType::USE_DATABASE;
        query.database = tokens[1];
    } else if (cmd == "INSERT") {
        query.type = QueryType::INSERT;
        std::string rest = cleaned.substr(cleaned.find("INTO") + 4);
        std::regex insert_regex(R"(\s*(\w+)\s*\((.*?)\)\s*VALUES\s*\((.*?)\))");
        std::smatch match;
        if (!std::regex_match(rest, match, insert_regex)) {
            throw std::runtime_error("Invalid INSERT syntax");
        }
        query.table = match[1];
        auto fields = split(match[2], ',');
        auto values = split(match[3], ',');
        if (fields.size() != values.size()) {
            throw std::runtime_error("Field and value count mismatch");
        }
        for (size_t i = 0; i < fields.size(); ++i) {
            query.insert_values.emplace_back(trim(fields[i]), trim(values[i]));
        }
    } else if (cmd == "SELECT") {
        query.type = QueryType::SELECT;
        std::string rest = cleaned.substr(cleaned.find("FROM"));
        std::regex select_regex(R"(\s*(.*?)\s*FROM\s*(\w+)(?:\s*WHERE\s*(\w+)\s*=\s*(\S+))?)");
        std::smatch match;
        if (!std::regex_match(rest, match, select_regex)) {
            throw std::runtime_error("Invalid SELECT syntax");
        }
        std::string fields_str = match[1];
        query.table = match[2];
        if (fields_str != "*") {
            query.select_fields = split(fields_str, ',');
        }
        if (match.size() > 3) {
            query.where_field = match[3];
            query.where_value = match[4];
        }
    } else if (cmd == "UPDATE") {
        query.type = QueryType::UPDATE;
        std::regex update_regex(R"(UPDATE\s+(\w+)\s+SET\s+(\w+)\s*=\s*(\S+)\s*WHERE\s+(\w+)\s*=\s*(\S+))");
        std::smatch match;
        if (!std::regex_match(cleaned, match, update_regex)) {
            throw std::runtime_error("Invalid UPDATE syntax");
        }
        query.table = match[1];
        query.update_values.emplace_back(match[2], match[3]);
        query.where_field = match[4];
        query.where_value = match[5];
    } else if (cmd == "DELETE") {
        query.type = QueryType::DELETE;
        std::regex delete_regex(R"(DELETE\s+FROM\s+(\w+)(?:\s*WHERE\s+(\w+)\s*=\s*(\S+))?)");
        std::smatch match;
        if (!std::regex_match(cleaned, match, delete_regex)) {
            throw std::runtime_error("Invalid DELETE syntax");
        }
        query.table = match[1];
        if (match.size() > 2) {
            query.where_field = match[2];
            query.where_value = match[3];
        }
    } else {
        query.type = QueryType::UNKNOWN;
        throw std::runtime_error("Unknown command: " + cmd);
    }

    if (!validateQuerySyntax(query)) {
        throw std::runtime_error("Invalid query syntax");
    }

    return query;
}

bool Parser::validateQuerySyntax(const Query& query) {
    switch (query.type) {
        case QueryType::CREATE_DATABASE:
        case QueryType::DROP_DATABASE:
        case QueryType::USE_DATABASE:
            return !query.database.empty() && isValidIdentifier(query.database);
        case QueryType::CREATE_TABLE:
            return !query.table.empty() && isValidIdentifier(query.table) &&
                   !query.fields.empty();
        case QueryType::DROP_TABLE:
        case QueryType::INSERT:
        case QueryType::SELECT:
        case QueryType::UPDATE:
        case QueryType::DELETE:
            return !query.table.empty() && isValidIdentifier(query.table);
        default:
            return false;
    }
}

} // namespace minisql