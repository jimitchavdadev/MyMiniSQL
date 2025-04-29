# MyMiniSQL

**MyMiniSQL** is a lightweight, educational SQL-like database management system implemented in **C++**. It provides a simplified interface for executing SQL commands, managing schemas, and persisting data in JSON format. Designed for learning and experimentation, it offers a REPL (Read-Eval-Print Loop) interface to interact with databases just like traditional SQL systems—without needing any external dependencies.


## 🚀 Project Objectives

- **Educational Tool**: Demonstrate the core components of a database system such as SQL parsing, schema definition, and persistent storage—ideal for students and developers.
- **Core SQL Functionality**: Support essential SQL-like commands:
  - `CREATE`, `DROP`, `USE`
  - `INSERT`, `SELECT`, `UPDATE`, `DELETE`
- **Lightweight & Portable**: All data is stored in JSON files. No external DB server needed.
- **Extensibility**: Modular code structure allows easy feature additions like new data types or query operations.
- **Robustness**: Basic error handling for invalid syntax, data types, and filesystem issues to ensure smooth user experience.



## 🧱 Code Structure

The project follows a modular directory structure:

```
MyMiniSQL/
│
├── src/                       # Source code
│   ├── main.cpp              # Entry point (REPL interface)
│   ├── parser.cpp/.hpp       # Query parsing logic
│   ├── executor.cpp/.hpp     # Query execution logic
│   ├── db_manager.cpp/.hpp   # Database and table management
│   ├── storage.cpp/.hpp      # JSON file handling
│   ├── utils.cpp/.hpp        # Helper functions
│   └── types.hpp             # Data type definitions
│
├── databases/                # Folder for JSON file storage
│   └── .gitkeep              # Ensures folder is tracked in Git
│
├── include/                  # External library headers
│   └── nlohmann/
│       └── json.hpp          # nlohmann/json library (header-only)
│
├── CMakeLists.txt            # Build configuration
└── README.md                 # Project documentation
```


## 📦 Dependencies

- C++17 or higher
- [nlohmann/json](https://github.com/nlohmann/json) (included in `include/`)



## ⚙️ Build & Run

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/MyMiniSQL.git
cd MyMiniSQL
```

### 2. Build with CMake

```bash
mkdir build && cd build
cmake ..
make
./MyMiniSQL
```

---

## 💬 Example Queries

```sql
CREATE DATABASE company;
USE company;
CREATE TABLE employees (id INT, name STRING, salary FLOAT);
INSERT INTO employees (id, name, salary) VALUES (1, 'Alice', 5000.50);
SELECT * FROM employees;
UPDATE employees SET salary = 6000 WHERE id = 1;
DELETE FROM employees WHERE name = 'Alice';
```


## 📚 License

This project is released under the MIT License.


## 🧠 Inspiration

MyMiniSQL is inspired by real-world DBMS systems and built as a hands-on way to explore how databases work under the hood.


## 👥 Contributing

Contributions, bug reports, and feature requests are welcome! Please open an issue or submit a pull request.

