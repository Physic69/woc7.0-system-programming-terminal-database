#ifndef DATABASE1_H
#define DATABASE1_H

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Record {
    int key;
    char value[100];
};

bool keyExists(const string& tableName, int searchKey) {
    ifstream file(tableName, ios::binary);
    if (!file) return false;

    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.key == searchKey) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void insertRecord(const string& tableName, int key, const string& value) {
    if (keyExists(tableName, key)) {
        cout << "Error: Key " << key << " already exists!\n";
        return;
    }
    
    ofstream file(tableName, ios::binary | ios::app);
    if (!file) {
        cerr << "Error opening file for writing!\n";
        return;
    }

    Record record = {key, ""};
    strncpy(record.value, value.c_str(), sizeof(record.value) - 1);
    file.write(reinterpret_cast<char*>(&record), sizeof(Record));
    file.close();
}

void listRecords(const string& tableName) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        cerr << "Error opening file for reading!\n";
        return;
    }

    Record record;
    cout << "---- Table Data (" << tableName << ") ----\n";
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        cout << "Key: " << record.key << " | Value: " << record.value << endl;
    }
    file.close();
}

void searchRecord(const string& tableName, int searchKey) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        cerr << "Error opening file for reading!\n";
        return;
    }
    
    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.key == searchKey) {
            cout << "Found! Key: " << record.key << " | Value: " << record.value << endl;
            return;
        }
    }
    cout << "Record not found!\n";
    file.close();
}

void updateRecord(const string& tableName, int searchKey, const string& newValue) {
    fstream file(tableName, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Error opening file for updating!\n";
        return;
    }
    
    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.key == searchKey) {
            strncpy(record.value, newValue.c_str(), sizeof(record.value) - 1);
            file.seekp(-static_cast<int>(sizeof(Record)), ios::cur);
            file.write(reinterpret_cast<char*>(&record), sizeof(Record));
            file.close();
            cout << "Record updated!\n";
            listRecords(tableName);
            return;
        }
    }
    
    cout << "Record not found!\n";
    file.close();
}

void deleteRecord(const string& tableName, int deleteKey) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        cerr << "Error opening file for deleting!\n";
        return;
    }

    ofstream temp("temp.dat", ios::binary);
    Record record;
    bool found = false;
    
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.key == deleteKey) {
            found = true;
        } else {
            temp.write(reinterpret_cast<char*>(&record), sizeof(Record));
        }
    }
    
    file.close();
    temp.close();
    
    remove(tableName.c_str());
    rename("temp.dat", tableName.c_str());
    
    if (found) {
        cout << "Record deleted!\n";
        listRecords(tableName);
    } else {
        cout << "Record not found!\n";
    }
}

#endif // DATABASE1_H

