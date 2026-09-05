#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include "network.h"
#include "monitor.h"

#define PORT 9090
#define BUFFER_SIZE 4096

void start_server(SensorData *machines, int machine_count)
{
#ifdef _WIN32

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Winsock initialization failed.\n");
        return;
    }

#endif

#ifdef _WIN32
    SOCKET server_socket;
    SOCKET client_socket;
#else
    int server_socket;
    int client_socket;
#endif

    struct sockaddr_in server_address;

    char buffer[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32

    if (server_socket == INVALID_SOCKET)
    {
        printf("Socket creation failed. Error: %d\n",
               WSAGetLastError());

        WSACleanup();
        return;
    }

#else

    if (server_socket < 0)
    {
        printf("Socket creation failed.\n");
        return;
    }

#endif

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if (bind(
            server_socket,
            (struct sockaddr *)&server_address,
            sizeof(server_address)) < 0)
    {
#ifdef _WIN32

        printf("Bind failed. Error: %d\n",
               WSAGetLastError());

        closesocket(server_socket);
        WSACleanup();

#else

        printf("Bind failed.\n");
        close(server_socket);

#endif

        return;
    }

    if (listen(server_socket, 5) < 0)
    {
#ifdef _WIN32

        printf("Listen failed. Error: %d\n",
               WSAGetLastError());

        closesocket(server_socket);
        WSACleanup();

#else

        printf("Listen failed.\n");
        close(server_socket);

#endif

        return;
    }

    printf("\n");
    printf("============================================\n");
    printf("        INDUSTRIAL IoT TCP SERVER\n");
    printf("============================================\n");
    printf("TCP server started successfully.\n");
    printf("Server address: 127.0.0.1\n");
    printf("Server port: 9090\n");
    printf("Waiting for clients...\n");

    while (1)
    {
        client_socket = accept(
            server_socket,
            NULL,
            NULL
        );

#ifdef _WIN32

        if (client_socket == INVALID_SOCKET)
        {
            printf("Accept failed. Error: %d\n",
                   WSAGetLastError());
            continue;
        }

#else

        if (client_socket < 0)
        {
            printf("Accept failed.\n");
            continue;
        }

#endif

        printf("\n");
        printf("TCP client connected.\n");

        memset(buffer, 0, sizeof(buffer));

        int offset = 0;

        offset += snprintf(
            buffer + offset,
            BUFFER_SIZE - offset,
            "============================================\n"
        );

        offset += snprintf(
            buffer + offset,
            BUFFER_SIZE - offset,
            "       INDUSTRIAL IoT SENSOR DATA\n"
        );

        offset += snprintf(
            buffer + offset,
            BUFFER_SIZE - offset,
            "============================================\n"
        );

        /*
         * Lock shared sensor data.
         */
        pthread_mutex_lock(&sensor_mutex);

        for (int i = 0; i < machine_count; i++)
        {
            offset += snprintf(
                buffer + offset,
                BUFFER_SIZE - offset,
                "\nMachine: %s\n",
                machines[i].machine_name
            );

            offset += snprintf(
                buffer + offset,
                BUFFER_SIZE - offset,
                "Temperature : %d C\n",
                machines[i].temperature
            );

            offset += snprintf(
                buffer + offset,
                BUFFER_SIZE - offset,
                "Vibration   : %d mm/s\n",
                machines[i].vibration
            );

            offset += snprintf(
                buffer + offset,
                BUFFER_SIZE - offset,
                "Pressure    : %d PSI\n",
                machines[i].pressure
            );

            offset += snprintf(
                buffer + offset,
                BUFFER_SIZE - offset,
                "Status      : %s\n",
                machines[i].status
            );
        }

        /*
         * Unlock after reading.
         */
        pthread_mutex_unlock(&sensor_mutex);

        offset += snprintf(
            buffer + offset,
            BUFFER_SIZE - offset,
            "\n============================================\n"
        );

#ifdef _WIN32

        int result = send(
            client_socket,
            buffer,
            offset,
            0
        );

        if (result == SOCKET_ERROR)
        {
            printf("Send failed. Error: %d\n",
                   WSAGetLastError());
        }
        else
        {
            printf("Sensor data sent to TCP client.\n");
        }

        closesocket(client_socket);

#else

        int result = send(
            client_socket,
            buffer,
            strlen(buffer),
            0
        );

        if (result < 0)
        {
            printf("Send failed.\n");
        }
        else
        {
            printf("Sensor data sent to TCP client.\n");
        }

        close(client_socket);

#endif

        printf("Client connection closed.\n");
        printf("Waiting for next client...\n");
    }

#ifdef _WIN32

    closesocket(server_socket);
    WSACleanup();

#else

    close(server_socket);

#endif
}