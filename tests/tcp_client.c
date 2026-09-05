#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 9090
#define BUFFER_SIZE 4096

int main(void)
{
    WSADATA wsa;

    SOCKET client_socket;

    struct sockaddr_in server_address;

    char buffer[BUFFER_SIZE];

    int bytes_received;


    printf("============================================\n");
    printf("       INDUSTRIAL IoT TCP CLIENT\n");
    printf("============================================\n");


    /*
     * Initialize Windows networking.
     */
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }


    /*
     * Create TCP socket.
     */
    client_socket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );


    if (client_socket == INVALID_SOCKET)
    {
        printf(
            "Socket creation failed. Error: %d\n",
            WSAGetLastError()
        );

        WSACleanup();

        return 1;
    }


    /*
     * Configure server address.
     */
    server_address.sin_family = AF_INET;

    server_address.sin_addr.s_addr =
        inet_addr(SERVER_IP);

    server_address.sin_port =
        htons(PORT);


    printf(
        "Connecting to %s:%d...\n",
        SERVER_IP,
        PORT
    );


    /*
     * Connect to Industrial IoT server.
     */
    if (connect(
            client_socket,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) == SOCKET_ERROR)
    {
        printf(
            "Connection failed. Error: %d\n",
            WSAGetLastError()
        );

        closesocket(client_socket);

        WSACleanup();

        return 1;
    }


    printf("Connected successfully!\n\n");


    /*
     * Receive sensor data.
     */
    memset(
        buffer,
        0,
        sizeof(buffer)
    );


    bytes_received = recv(
        client_socket,
        buffer,
        BUFFER_SIZE - 1,
        0
    );


    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        printf("Sensor data received:\n\n");

        printf("%s\n", buffer);
    }
    else if (bytes_received == 0)
    {
        printf("Server closed the connection.\n");
    }
    else
    {
        printf(
            "Receive failed. Error: %d\n",
            WSAGetLastError()
        );
    }


    /*
     * Close client socket.
     */
    closesocket(client_socket);

    WSACleanup();


    printf("\nTCP client finished.\n");

    return 0;
}