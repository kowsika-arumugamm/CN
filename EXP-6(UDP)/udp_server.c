#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <math.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

void func(SOCKET sockfd, struct sockaddr_in *servaddr);

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in servaddr;

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

    func(sockfd, &servaddr);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

void func(SOCKET sockfd, struct sockaddr_in *servaddr) {
    int numbers[2];
    printf("Client: Enter two numbers (base and exponent): ");
    scanf("%d %d", &numbers[0], &numbers[1]);

    // Send data to server
    int sentBytes = sendto(sockfd, (char*)&numbers, sizeof(numbers), 0, (struct sockaddr*)servaddr, sizeof(*servaddr));
    if (sentBytes == SOCKET_ERROR) {
        printf("Send failed. Error Code: %d\n", WSAGetLastError());
        return;
    }

    float result;
    int addr_len = sizeof(*servaddr);
    // Receive result from server
    int receivedBytes = recvfrom(sockfd, (char*)&result, sizeof(result), 0, (struct sockaddr*)servaddr, &addr_len);
    if (receivedBytes == SOCKET_ERROR) {
        printf("Receive failed. Error Code: %d\n", WSAGetLastError());
        return;
    }
    printf("Result: %.2f\n", result);
}