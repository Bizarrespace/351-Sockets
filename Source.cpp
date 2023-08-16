#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>


// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSAData WSAData;

    SOCKET SendSocket;
    SOCKET RecvSocket;

    sockaddr_in sender;
    int SenderAddrSize = sizeof(sender);

    sockaddr_in dest;

    const char* srcIP = "127.0.0.1";
    const char* destIP = "127.0.0.1";

    std::string message;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &WSAData);

    //---------------------------------------------
    // Set up sender structure
    sender.sin_family = AF_INET;
    inet_pton(AF_INET, srcIP, &sender.sin_addr.s_addr);
    sender.sin_port = htons(3514);


    // Set up the RecvAddr structure
    dest.sin_family = AF_INET;
    inet_pton(AF_INET, destIP, &dest.sin_addr.s_addr);
    dest.sin_port = htons(3515);

    //Make socket, bind it and then sendto
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(SendSocket, (SOCKADDR*)&sender, sizeof(sender));

    RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(RecvSocket, (SOCKADDR*)&dest, sizeof(dest));


    while (message != "quit") {
        std::cout << "=========================\n";
        std::cout << "      UDP Client App\n";
        std::cout << "=========================\n\n";
        std::cout << "Client(Type quit to exit)\n";
        std::getline(std::cin, message);
        if (message == "quit") {
            break;
        }
        const char* pkt = message.c_str();

        char RecvBuf[1024];
        strcpy_s(RecvBuf, pkt);
        int BufLen = 1024;


        //Set up sendto function to send the message
        sendto(SendSocket,
            pkt, strlen(pkt), 0, (SOCKADDR*)&dest, sizeof(dest));


        //Receive from and print out what was sent
        recvfrom(RecvSocket,
            RecvBuf, BufLen, 0, (SOCKADDR*)&sender, &SenderAddrSize);
        std::cout << "\n\nServer:\n";
        std::cout << "\033[1;32m" << RecvBuf << "\033[0m\n\n";
        Sleep(2500);
        system("cls");

    }

    //---------------------------------------------
    // Clean up and quit.
    closesocket(SendSocket);
    closesocket(RecvSocket);

    wprintf(L"Exiting.\n");
    WSACleanup();
    return 0;
}