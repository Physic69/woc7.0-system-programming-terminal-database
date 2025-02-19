#include "database1.h"
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 8080

void sendMessage(int clientSocket, const string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}

void printUsage(int clientSocket) {
    string usage = "Commands:\n";
    usage += "  insert <key> <value>   - Insert a new record\n";
    usage += "  list                    - List all records\n";
    usage += "  search <key>            - Search for a record\n";
    usage += "  update <key> <value>    - Update a record\n";
    usage += "  delete <key>            - Delete a record\n";
    usage += "  sortb   	         - Sort in descending order\n";
    usage += "  sorta	            - Sort in ascending order\n";
    usage += "  help                    - Show this message\n";
    sendMessage(clientSocket, usage);
}

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    string tableName = "tabledata.dat";
    int bytesReceived = read(clientSocket, buffer, sizeof(buffer));
    if (bytesReceived <= 0) {
        cerr << "Error receiving command from client." << endl;
        return;
    }
    
    string command(buffer, bytesReceived);
    stringstream ss(command);
    string cmd, response;
    ss >> cmd;
    for (char& c : cmd) c = tolower(c);

    if (cmd == "insert") {
        int key;
        string value;
        ss >> key >> value;
        bool t=insertRecord(tableName, key, value);
        if (t== true){
        response = "Record inserted successfully!";}
        else{
        	response= " Either error in opening file or key already exists or enter key or value properlyls
        	";
        }
    }
    else if (cmd == "sorta" || cmd=="sort") {
        response = sortRecords(tableName);
    } else if (cmd == "sortd") {
        response = sortRecordsDescending(tableName);
    } else if (cmd == "list") {
        response = listRecords(tableName);
    } else if (cmd == "search") {
        int key;
        ss >> key;
        response = searchRecord(tableName, key);
    } else if (cmd == "update") {
        int key;
        string value;
        ss >> key >> value;
        updateRecord(tableName, key, value);
        response = "Record updated successfully!";
    } else if (cmd == "delete") {
        int key;
        ss >> key;
        deleteRecord(tableName, key);
        response = "Record deleted successfully!";
    } else if (cmd == "help") {
        printUsage(clientSocket);
        return;
    } else {
        response = "Unknown command. Use 'help' for usage information.";
    }
    
    sendMessage(clientSocket, response + "\n");
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket!" << endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket!" << endl;
        return -1;
    }

    if (listen(serverSocket, 5) < 0) {
        cerr << "Error listening!" << endl;
        return -1;
    }

    cout << "Server listening on port " << PORT << "..." << endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket < 0) {
            cerr << "Error accepting client!" << endl;
            continue;
        }
        handleClient(clientSocket);
    }

    close(serverSocket);
    return 0;
}

