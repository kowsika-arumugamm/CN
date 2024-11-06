#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void func(SOCKET confd);

int main() {
    WSADATA wsa;
    SOCKET sockfd, confd;
    struct sockaddr_in servaddr, cliaddr;
    int len;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Winsock initialized.\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket is created\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(43451);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Bind successfully\n");

    if (listen(sockfd, 5) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Listen successfully\n");

    len = sizeof(cliaddr);
    confd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if (confd == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Server accepted successfully\n");
    func(confd);

    closesocket(confd);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}

void func(SOCKET confd) {
    int n, fib[100];

    printf("The value for number of terms N:\n");
    recv(confd, (char*)&n, sizeof(n), 0);
    printf("%d\n", n);

    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    printf("The Fibonacci series is:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", fib[i]);
    }
    printf("\n");

    send(confd, (char*)fib, n * sizeof(int), 0);
}
