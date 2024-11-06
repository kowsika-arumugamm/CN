#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <math.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

void func(SOCKET sockfd, struct sockaddr_in *cliaddr);

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int len = sizeof(cliaddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error Code : %d\n", WSAGetLastError());
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return -1;
    } else {
        printf("Socket created\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(43451);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(sockfd);
        WSACleanup();
        return -1;
    } else {
        printf("Bind successfully\n");
    }

    while (1) {
        func(sockfd, &cliaddr);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

void func(SOCKET sockfd, struct sockaddr_in *cliaddr) {
    int numbers[2];
    float result;
    int addr_len = sizeof(*cliaddr);

    // Receive data from client
    int receivedBytes = recvfrom(sockfd, (char*)&numbers, sizeof(numbers), 0, (struct sockaddr*)cliaddr, &addr_len);
    if (receivedBytes == SOCKET_ERROR) {
        printf("Receive failed. Error Code: %d\n", WSAGetLastError());
        return;
    }

    printf("Received numbers: %d (base), %d (exponent)\n", numbers[0], numbers[1]);

    // Calculate power
    result = pow(numbers[0], numbers[1]);
    // Send result back to client
    int sentBytes = sendto(sockfd, (char*)&result, sizeof(result), 0, (struct sockaddr*)cliaddr, addr_len);
    if (sentBytes == SOCKET_ERROR) {
        printf("Send failed. Error Code: %d\n", WSAGetLastError());
        return;
    }
}