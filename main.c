#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "sensor.h"
#include "database.h"
#include "monitor.h"
#include "network.h"


/*
 * TCP/IP network thread
 */
void *network_thread(void *arg)
{
    SensorData *machines = (SensorData *)arg;

    start_server(machines, 3);

    return NULL;
}


int main(void)
{
    sqlite3 *db = NULL;

    SensorData machines[3];

    pthread_t threads[3];
    pthread_t network_tid;

    MonitorArgs args[3];


    /*
     * Set machine names
     */
    strcpy(machines[0].machine_name, "Motor-A");
    strcpy(machines[1].machine_name, "Pump-B");
    strcpy(machines[2].machine_name, "Compressor-C");


    /*
     * Initialize random number generator
     */
    srand((unsigned int)time(NULL));


    /*
     * Display project information
     */
    printf("============================================\n");
    printf("     INDUSTRIAL IoT MONITORING SYSTEM\n");
    printf("============================================\n");

    printf("Machines monitored: 3\n");

    printf("Motor-A | Pump-B | Compressor-C\n\n");


    /*
     * Initialize SQLite database
     */
    if (!initialize_database(&db))
    {
        printf("Database initialization failed.\n");

        return 1;
    }

    printf("Database connected successfully.\n");


    /*
     * Start TCP/IP server thread
     *
     * Passing the machines array to the
     * network thread allows the TCP server
     * to send real sensor information.
     */
    printf("Starting TCP/IP network server...\n");

    if (pthread_create(
            &network_tid,
            NULL,
            network_thread,
            machines) != 0)
    {
        printf("Failed to create network thread.\n");

        close_database(&db);

        return 1;
    }


    /*
     * Start real-time monitoring threads
     */
    printf("Starting real-time monitoring threads...\n");

    for (int i = 0; i < 3; i++)
    {
        args[i].data = &machines[i];

        args[i].db = db;


        if (pthread_create(
                &threads[i],
                NULL,
                monitor_machine,
                &args[i]) != 0)
        {
            printf(
                "Failed to create thread for %s\n",
                machines[i].machine_name
            );

            return 1;
        }


        printf(
            "Thread started for %s\n",
            machines[i].machine_name
        );
    }


    /*
     * Wait for all machine monitoring threads.
     *
     * These threads continuously monitor
     * the machines.
     */
    for (int i = 0; i < 3; i++)
    {
        pthread_join(
            threads[i],
            NULL
        );
    }


    /*
     * Wait for TCP/IP network thread.
     */
    pthread_join(
        network_tid,
        NULL
    );


    /*
     * Close database.
     */
    close_database(&db);


    return 0;
}