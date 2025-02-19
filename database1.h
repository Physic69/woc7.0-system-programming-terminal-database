#ifndef DATABASE1_H
#define DATABASE1_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
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
bool insertRecord(const string& tableName, int key, const string& value) {
    if (keyExists(tableName, key)) {
        cout << "Error: Key " << key << " already exists!\n";
        return 0;
    }   
    ofstream file(tableName, ios::binary | ios::app);
    if (!file) {
        cerr << "Error opening file for writing!\n";
        return 0;
    }
    if(key <=0)
    {            cerr << "Error inserting in file\n";
    return 0;}
    if( value.empty()){
            cerr << "Error inserting in file\n";
    return false;
    }
    Record record = {key, ""};
    strncpy(record.value, value.c_str(), sizeof(record.value) - 1);
    file.write(reinterpret_cast<char*>(&record), sizeof(Record));
    file.close();
    return 1;
}
string listRecords(const string& tableName) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        return "Error opening file for reading!\n";
    }
    Record record;
    string result = "---- Table Data (" + tableName + ") ----\n";
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        result += "Key: " + to_string(record.key) + " | Value: " + record.value + "\n";
    }
    file.close();
    return result;
}
string sortRecordsDescending(const string& tableName) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        return "Error opening file for reading!\n";
    }

    vector<Record> records;
    Record record;


    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        records.push_back(record);
    }
    file.close();


    sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        return a.key > b.key;  
    });


    string result = "---- Sorted Records (Descending Order) ----\n";
    for (const auto& rec : records) {
        result += "Key: " + to_string(rec.key) + " | Value: " + rec.value + "\n";
    }

    return result;
}

string sortRecords(const string& tableName) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        return "Error opening file for reading!\n";
    }

    vector<Record> records;
    Record record;


    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        records.push_back(record);
    }
    file.close();


    sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        return a.key < b.key;  
    });


    string result = "---- Sorted Records (Descending Order) ----\n";
    for (const auto& rec : records) {
        result += "Key: " + to_string(rec.key) + " | Value: " + rec.value + "\n";
    }

    return result;
}

string searchRecord(const string& tableName, int searchKey) {
    ifstream file(tableName, ios::binary);
    if (!file) {
        return "Error opening file for reading!\n";
    }
    Record record;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        if (record.key == searchKey) {
            file.close();
            return "Found! Key: " + to_string(record.key) + " | Value: " + record.value + "\n";
        }
    }
    file.close();
    return "Record not found!\n";
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

