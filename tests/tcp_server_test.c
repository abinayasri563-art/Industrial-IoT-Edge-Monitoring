#include <stdio.h>
#include <winsock2.h>

#define PORT 9090

int main(void)
{
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server_address;

    printf("Starting TCP server test...\n");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET)
    {
        printf("Socket creation failed: %d\n",
               WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if (bind(
            server_socket,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) == SOCKET_ERROR)
    {
        printf("Bind failed: %d\n",
               WSAGetLastError());

        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR)
    {
        printf("Listen failed: %d\n",
               WSAGetLastError());

        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("TCP TEST SERVER IS LISTENING.\n");
    printf("Port: 9090\n");
    printf("Keep this terminal open.\n");

    while (1)
    {
        Sleep(1000);
    }

    return 0;
}