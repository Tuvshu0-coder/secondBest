#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsa;
    cout << "Connecting to server..." << endl;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cout << "Failed. Error Code : " << WSAGetLastError();
        return 1;
    }

    SOCKET s;
    sockaddr_in server;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Could not create socket: " << WSAGetLastError();
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // connect to localhost
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cout << "Connect error: " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!" << endl;

    char server_reply[2000];
    int recv_size = recv(s, server_reply, sizeof(server_reply), 0);
    if (recv_size == SOCKET_ERROR) {
        cout << "Recv failed: " << WSAGetLastError();
    } else {
        server_reply[recv_size] = '\0';
        cout << "Server says: " << server_reply << endl;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
