#include "db_manager.hpp"
#include "storage.hpp"
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace minisql {

DatabaseManager::DatabaseManager(const std::string& base_path) : base_path_(base_path) {
    // Ensure the base path (./databases) exists
    try {
        std::filesystem::create_directories(base_path_);
    } catch (const std::filesystem::filesystem_error& e) {
        throw std::runtime_error("Failed to create base directory '" + base_path_ + "': " + e.what());
    }
}

void DatabaseManager::setCurrentDatabase(const std::string& db_name) {
    if (!std::filesystem::exists(base_path_ + "/" + db_name)) {
        throw std::runtime_error("Database does not exist: " + db_name);
    }
    current_db_ = db_name;
}

std::string DatabaseManager::getCurrentDatabase() const {
    return current_db_;
}

void DatabaseManager::executeQuery(const Query& query) {
    if (query.type != QueryType::CREATE_DATABASE && query.type != QueryType::DROP_DATABASE &&
        query.type != QueryType::USE_DATABASE && current_db_.empty()) {
        throw std::runtime_error("No database selected. Use 'USE database;'");
    }

    switch (query.type) {
        case QueryType::CREATE_DATABASE:
            createDatabase(query.database);
            break;
        case QueryType::DROP_DATABASE:
            dropDatabase(query.database);
            break;
        case QueryType::USE_DATABASE:
            setCurrentDatabase(query.database);
            break;
        case QueryType::CREATE_TABLE:
            createTable(query);
            break;
        case QueryType::DROP_TABLE:
            dropTable(query.table);
            break;
        case QueryType::INSERT:
            insert(query);
            break;
        case QueryType::SELECT:
            select(query);
            break;
        case QueryType::UPDATE:
            update(query);
            break;
        case QueryType::DELETE:
            deleteFrom(query);
            break;
        default:
            throw std::runtime_error("Unsupported query type");
    }
}

void DatabaseManager::createDatabase(const std::string& db_name) {
    try {
        std::filesystem::create_directory(base_path_ + "/" + db_name);
        std::cout << "Database " << db_name << " created.\n";
    } catch (const std::filesystem::filesystem_error& e) {
        throw std::runtime_error("Failed to create database '" + db_name + "': " + e.what());
    }
}

void DatabaseManager::dropDatabase(const std::string& db_name) {
    std::filesystem::remove_all(base_path_ + "/" + db_name);
    if (current_db_ == db_name) {
        current_db_.clear();
    }
    std::cout << "Database " << db_name << " dropped.\n";
}

void DatabaseManager::createTable(const Query& query) {
    TableSchema schema;
    for (const auto& field : query.fields) {
        schema.fields.push_back(field);
        schema.field_types[field.name] = field.type;
    }
    saveTableSchema(query.table, schema);
    tables_[query.table] = schema;
    std::cout << "Table " << query.table << " created.\n";
}

void DatabaseManager::dropTable(const std::string& table_name) {
    std::filesystem::remove(base_path_ + "/" + current_db_ + "/" + table_name + ".json");
    tables_.erase(table_name);
    std::cout << "Table " << table_name << " dropped.\n";
}

void DatabaseManager::insert(const Query& query) {
    auto schema = loadTableSchema(query.table);
    json data = Storage::loadTableData(current_db_, query.table, base_path_);
    json row;

    for (const auto& [field, value] : query.insert_values) {
        auto it = schema.field_types.find(field);
        if (it == schema.field_types.end()) {
            throw std::runtime_error("Unknown field: " + field);
        }
        if (!validateValue(value, it->second)) {
            throw std::runtime_error("Invalid value for field " + field);
        }
        row[field] = value;
    }

    if (!data.is_array()) {
        data = json::array();
    }
    data.push_back(row);
    Storage::saveTableData(current_db_, query.table, data, base_path_);
    std::cout << "1 row inserted.\n";
}

void DatabaseManager::select(const Query& query) {
    auto schema = loadTableSchema(query.table);
    json data = Storage::loadTableData(current_db_, query.table, base_path_);
    std::vector<std::string> fields;
    if (query.select_fields.empty()) {
        fields.reserve(schema.field_types.size());
        std::transform(schema.field_types.begin(), schema.field_types.end(),
                       std::back_inserter(fields),
                       [](const auto& pair) { return pair.first; });
    } else {
        fields = query.select_fields;
    }

    // Print header
    for (size_t i = 0; i < fields.size(); ++i) {
        std::cout << std::setw(15) << fields[i];
        if (i < fields.size() - 1) std::cout << "|";
    }
    std::cout << "\n" << std::string(15 * fields.size() + fields.size() - 1, '-') << "\n";

    // Print rows
    for (const auto& row : data) {
        if (!query.where_field.empty()) {
            if (row[query.where_field].get<std::string>() != query.where_value) {
                continue;
            }
        }
        for (size_t i = 0; i < fields.size(); ++i) {
            std::cout << std::setw(15) << row[fields[i]].get<std::string>();
            if (i < fields.size() - 1) std::cout << "|";
        }
        std::cout << "\n";
    }
}

void DatabaseManager::update(const Query& query) {
    auto schema = loadTableSchema(query.table);
    json data = Storage::loadTableData(current_db_, query.table, base_path_);
    int updated = 0;

    for (auto& row : data) {
        if (row[query.where_field].get<std::string>() == query.where_value) {
            for (const auto& [field, value] : query.update_values) {
                auto it = schema.field_types.find(field);
                if (it == schema.field_types.end()) {
                    throw std::runtime_error("Unknown field: " + field);
                }
                if (!validateValue(value, it->second)) {
                    throw std::runtime_error("Invalid value for field " + field);
                }
                row[field] = value;
            }
            ++updated;
        }
    }

    Storage::saveTableData(current_db_, query.table, data, base_path_);
    std::cout << updated << " rows updated.\n";
}

void DatabaseManager::deleteFrom(const Query& query) {
    auto schema = loadTableSchema(query.table);
    json data = Storage::loadTableData(current_db_, query.table, base_path_);
    json new_data = json::array();
    int deleted = 0;

    for (const auto& row : data) {
        if (query.where_field.empty() || row[query.where_field].get<std::string>() != query.where_value) {
            new_data.push_back(row);
        } else {
            ++deleted;
        }
    }

    Storage::saveTableData(current_db_, query.table, new_data, base_path_);
    std::cout << deleted << " rows deleted.\n";
}

DatabaseManager::TableSchema DatabaseManager::loadTableSchema(const std::string& table_name) {
    auto it = tables_.find(table_name);
    if (it != tables_.end()) {
        return it->second;
    }
    auto schema = Storage::loadTableSchema(current_db_, table_name, base_path_);
    TableSchema result;
    for (const auto& field : schema["fields"]) {
        TableField f;
        f.name = field["name"].get<std::string>();
        f.type = stringToDataType(field["type"].get<std::string>());
        result.fields.push_back(f);
        result.field_types[f.name] = f.type;
    }
    tables_[table_name] = result;
    return result;
}

void DatabaseManager::saveTableSchema(const std::string& table_name, const TableSchema& schema) {
    json schema_json;
    schema_json["fields"] = json::array();
    for (const auto& field : schema.fields) {
        json f;
        f["name"] = field.name;
        f["type"] = dataTypeToString(field.type);
        schema_json["fields"].push_back(f);
    }
    Storage::saveTableSchema(current_db_, table_name, schema_json, base_path_);
}

} // namespace minisql