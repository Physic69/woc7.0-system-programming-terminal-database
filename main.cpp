#include "database1.h"
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // for inet_pton
using namespace std;

#define PORT 8080

// Function to send messages to the client
void sendMessage(int clientSocket, const string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}

// Print usage function that sends the usage to the client
void printUsage(int clientSocket) {
    string usage = "Commands:\n";
    usage += "  insert <key> <value>   - Insert a new record\n";
    usage += "  list                    - List all records\n";
    usage += "  search <key>            - Search for a record\n";
    usage += "  update <key> <value>    - Update a record\n";
    usage += "  delete <key>            - Delete a record\n";
    usage += "  help                    - Show this message\n";
    sendMessage(clientSocket, usage);  // Send usage message to client
}

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    string tableName = "tabledata.dat";

    // Receive command from the client
    int bytesReceived = read(clientSocket, buffer, sizeof(buffer));
    if (bytesReceived <= 0) {
        cerr << "Error receiving command from client." << endl;
        return;
    }
    string command = string(buffer, bytesReceived);

    // Process the command
    stringstream ss(command);
    string cmd;
    ss >> cmd;
    
    for (char& c : cmd) {
        c = tolower(c);
    }

    if (cmd == "insert") {
        int key;
        string value;
        ss >> key >> value;
        insertRecord(tableName, key, value);
        listRecords(tableName);
    } else if (cmd == "list") {
        listRecords(tableName);
    } else if (cmd == "search") {
        int key;
        ss >> key;
        searchRecord(tableName, key);
    } else if (cmd == "update") {
        int key;
        string value;
        ss >> key >> value;
        updateRecord(tableName, key, value);
    } else if (cmd == "delete") {
        int key;
        ss >> key;
        deleteRecord(tableName, key);
    } else if (cmd == "help") {
        printUsage(clientSocket);  // Send usage message to client
    } else {
        string errorMessage = "Unknown command '" + cmd + "'. Use 'help' for usage information.";
        sendMessage(clientSocket, errorMessage);  // Send error message to client
    }

    close(clientSocket);  // Close the connection after handling the request
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket!" << endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket to the address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket!" << endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        cerr << "Error listening!" << endl;
        return -1;
    }

    cout << "Server is listening on port " << PORT << "..." << endl;

    // Accept client connections in an infinite loop
    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket < 0) {
            cerr << "Error accepting client!" << endl;
            continue;
        }

        // Handle the client
        handleClient(clientSocket);
    }

    close(serverSocket);
    return 0;
}

