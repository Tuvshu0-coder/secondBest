#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    string message;

    cout << "Client connected! Type 'exit' to close the server.\n";

    // Thread to receive messages from client
    thread recvThread([&]() {
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) break;
            buffer[bytesReceived] = '\0';
            cout << "\nClient: " << buffer << endl;
            if (string(buffer) == "exit") break;
        }
    });

    // Main thread: send messages
    while (true) {
        cout << "You: ";
        getline(cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
        if (message == "exit") break;
    }

    closesocket(clientSocket);
    recvThread.join();
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(54000);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    cout << "Server is running. Waiting for connection...\n";

    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    handleClient(clientSocket);

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
