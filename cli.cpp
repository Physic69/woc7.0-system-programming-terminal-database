#include <iostream>
#include "database.h"

using namespace std;

void showMenu() {
    cout << "\nCommands:\n";
    cout << "1. insert <key> <value>\n";
    cout << "2. update <key> <new_value>\n";
    cout << "3. delete <key>\n";
    cout << "4. display\n";
    cout << "5. exit\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./database <database_name> <table_name>\n";
        return 1;
    }

    string db_name = argv[1];
    string table_name = argv[2];

    Database db(db_name);
    db.createTable(table_name);  // Ensure table exists

    string command;
    showMenu();
    
    while (true) {
        cout << "\n> ";
        cin >> command;

        if (command == "insert") {
            string key, value;
            cin >> key;
            getline(cin >> ws, value);
            db.insertData(table_name, key, value);
        }
        else if (command == "update") {
            string key, new_value;
            cin >> key;
            getline(cin >> ws, new_value);
            db.updateEntry(table_name, key, new_value);
        }
        else if (command == "delete") {
            string key;
            cin >> key;
            db.deleteEntry(table_name, key);
        }
        else if (command == "display") {
            db.displayTable(table_name);
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Invalid command! Try again.\n";
            showMenu();
        }
    }

    return 0;
}
