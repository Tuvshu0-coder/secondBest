#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);

    connect(s, (sockaddr*)&server, sizeof(server));
    cout << "Connected to server! Type 'exit' to quit.\n";

    char buffer[1024];
    string message;

    // Thread for receiving messages
    thread recvThread([&]() {
        while (true) {
            int bytesReceived = recv(s, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) break;
            buffer[bytesReceived] = '\0';
            cout << "\nServer: " << buffer << endl;
            if (string(buffer) == "exit") break;
        }
    });

    // Main thread for sending messages
    while (true) {
        cout << "You: ";
        getline(cin, message);
        send(s, message.c_str(), message.size(), 0);
        if (message == "exit") break;
    }

    closesocket(s);
    recvThread.join();
    WSACleanup();
    return 0;
}
