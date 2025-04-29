# MyMiniSQL

A lightweight MySQL-like DBMS implemented in C++ with JSON-based storage.

## Features****
- Supports basic SQL-like queries: `CREATE`, `DROP`, `INSERT`, `SELECT`, `UPDATE`, `DELETE`.
- Stores data and metadata in JSON files.
- Supports data types: `STRING`, `INT`, `FLOAT`, `BOOLEAN`.
- Command-line REPL interface.
- Basic `WHERE` clause for filtering.

## Prerequisites
- C++17 compiler (e.g., g++, clang++)
- CMake 3.10+
- nlohmann/json (included in `include/nlohmann/`)

## Build Instructions
1. Clone the repository:
   ```bash
   git clone <repo-url>
   cd MyMiniSQL