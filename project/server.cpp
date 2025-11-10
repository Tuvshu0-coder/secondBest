#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsa; // WSADATA is structure
    cout << "Initializing Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cout << "Failed. Error Code : " << WSAGetLastError();
        return 1;
    }
    //1. int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
    //2. wVersionRequested : The highest version of Windows Sockets specification that the caller can use. 
    //   The high-order byte specifies the minor version number; the low-order byte specifies the major version number
    //3. A pointer to the LPWSADATA data structure that is to recieve details of the Windows Sockets implementation
    //4. If successfil, the WSAStartup function reurns zero.



    cout << "Winsock initialized." << endl;

    SOCKET serverSocket;
    sockaddr_in serverAddr; //SOCKADDR_IN is structure

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Could not create socket: " << WSAGetLastError();
        return 1;
    }
    //1. socket(int af, int type, int protocol); 
    //2. af      :The adress family specification(AF_INET FOR UDP OR TCP).
    //3. type    : The type specification for the new socket (SOCK_STREAM for TCP, SOCK_DGRAM for UDP).
    //4. protocol: The protocol to be used (IPPROTO_TCP for TCP).


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // any IP
    serverAddr.sin_port = htons(54000); // port 54000
    //The SOCKADDR_IN structure is used by the Windows Sockets (IP4) to speficy a loal or remote endpoint adress to which to connect a socket
    //sin_family : Adress family (must be AF_INET).
    //sin_port   :IP port.
    //sin_addr   :IP Address 
    //sin_zero   :Padding to make sure structure the same size as SOCKADDR. The htons returns the value in TCP/IP network byte order. 

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind failed with error code: " << WSAGetLastError();
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    //1. int bind(SOCKET s, const struct sockaddr* name, int socklen)
    //2. s : Descriptor identifying an unbound socket.
    //3. name : Adress to assign to the socket from the sockaddr structure.
    //4. socklen : Length in bytes of the address structure.
    //5. if no error occurs, bind() returns zero. Otherwise, it returns SOCKET_ERROR

    listen(serverSocket, 3);
    cout << "Waiting for incoming connections..." << endl;

    sockaddr_in clientAddr;
    int c = sizeof(sockaddr_in);
    SOCKET newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &c);

    if (newSocket == INVALID_SOCKET) {
        cout << "Accept failed: " << WSAGetLastError();
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Client connected!" << endl;

    const char *message = "Hi Lalraa";
    send(newSocket, message, strlen(message), 0);

    closesocket(newSocket);
    closesocket(serverSocket);
    WSACleanup();
    cout << "Server closed." << endl;

    return 0;
}
