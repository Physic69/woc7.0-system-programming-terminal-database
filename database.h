#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>

using namespace std;
namespace fs = filesystem;

class Database {
private:
    string db_path;

public:
    Database(const string& db_name) {
        db_path = "./" + db_name;
        if (!fs::exists(db_path)) {
            fs::create_directory(db_path);
        }
    }

    void createTable(const string& table_name) {
        string table_file = db_path + "/" + table_name + ".json";
        if (!fs::exists(table_file)) {
            ofstream file(table_file);
            nlohmann::json empty_json = {};
            file << empty_json.dump(4);
            file.close();
            cout << "Table '" << table_name << "' created successfully.\n";
        } else {
            cout << "Table '" << table_name << "' already exists.\n";
        }
    }

    void insertData(const string& table_name, const string& key, const string& value) {
        string table_file = db_path + "/" + table_name + ".json";
        ifstream file_in(table_file);
        if (!file_in) {
            cout << "Table does not exist.\n";
            return;
        }

        nlohmann::json table_data;
        file_in >> table_data;
        file_in.close();

        if (table_data.contains(key)) {
            cout << "Key already exists!\n";
            return;
        }

        table_data[key] = value;
        ofstream file_out(table_file);
        file_out << table_data.dump(4);
        file_out.close();
        cout << "Data inserted successfully.\n";
    }

    void listTables() {
        for (const auto& entry : fs::directory_iterator(db_path)) {
            cout << "Table: " << entry.path().filename().stem().string() << "\n";
        }
    }

    void displayTable(const string& table_name) {
        string table_file = db_path + "/" + table_name + ".json";
        ifstream file(table_file);
        if (!file) {
            cout << "Table does not exist.\n";
            return;
        }

        nlohmann::json table_data;
        file >> table_data;
        file.close();

        cout << table_data.dump(4) << "\n";
    }

    void updateEntry(const string& table_name, const string& key, const string& new_value) {
        string table_file = db_path + "/" + table_name + ".json";
        ifstream file_in(table_file);
        if (!file_in) {
            cout << "Table does not exist.\n";
            return;
        }

        nlohmann::json table_data;
        file_in >> table_data;
        file_in.close();

        if (!table_data.contains(key)) {
            cout << "Key does not exist!\n";
            return;
        }

        table_data[key] = new_value;
        ofstream file_out(table_file);
        file_out << table_data.dump(4);
        file_out.close();
        cout << "Data updated successfully.\n";
    }

    void deleteEntry(const string& table_name, const string& key) {
        string table_file = db_path + "/" + table_name + ".json";
        ifstream file_in(table_file);
        if (!file_in) {
            cout << "Table does not exist.\n";
            return;
        }

        nlohmann::json table_data;
        file_in >> table_data;
        file_in.close();

        if (!table_data.contains(key)) {
            cout << "Key does not exist!\n";
            return;
        }

        table_data.erase(key);
        ofstream file_out(table_file);
        file_out << table_data.dump(4);
        file_out.close();
        cout << "Data deleted successfully.\n";
    }
};

#endif // DATABASE_H
