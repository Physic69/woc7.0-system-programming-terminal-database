#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 
using namespace std;
#define SERVER_ADDR "127.0.0.1" // inet
#define PORT 8080
void sendCommandToServer(const string& command) {
    int sock;
    struct sockaddr_in serverAddr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error creating socket!" << endl;
        return;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr) <= 0) {
        cerr << "Invalid address!" << endl;
        close(sock);
        return;
    }
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {//connectn
        cerr << "Connection failed!" << endl;
        close(sock);
        return;
    }
    send(sock, command.c_str(), command.size(), 0);// send data
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);// ser resp
    cout << "Server Response: " << string(buffer, valread) << endl;
    close(sock);
}
int main() {
    string command;
    cout<<"Enter help to get all the commands, HELP/help/Help/hELp/... anything which spells help is fine"<<endl;
    while (true) {
        cout << "Enter command : ";
        getline(cin, command);  
      if (command == "exit") {
            cout << "Exiting client..." << endl;
            break;
       }    
        sendCommandToServer(command);
    }
    return 0;
}

