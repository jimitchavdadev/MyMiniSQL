#include "parser.hpp"
#include "db_manager.hpp"
#include <iostream>
#include <string>

int main() {
    minisql::DatabaseManager db_manager("./databases");
    minisql::Parser parser;
    std::string input;

    std::cout << "MyMiniSQL REPL (type 'exit' to quit)\n> ";
    while (std::getline(std::cin, input)) {
        if (input == "exit") break;
        if (input.empty()) {
            std::cout << "> ";
            continue;
        }
        try {
            auto query = parser.parse(input);
            db_manager.executeQuery(query);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }

        std::cout << "> ";
    }

    return 0;
}