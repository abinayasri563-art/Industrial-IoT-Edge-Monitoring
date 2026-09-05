#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "monitor.h"
#include "sensor.h"
#include "fault_detection.h"
#include "database.h"
#include "logger.h"
#include "mqtt.h"


/*
 * Shared mutex protects sensor data.
 */
pthread_mutex_t sensor_mutex = PTHREAD_MUTEX_INITIALIZER;


void *monitor_machine(void *arg)
{
    MonitorArgs *args = (MonitorArgs *)arg;

    SensorData *data = args->data;
    sqlite3 *db = args->db;


    while (1)
    {
        /*
         * Lock shared sensor data.
         */
        pthread_mutex_lock(&sensor_mutex);


        /*
         * Generate new sensor readings.
         */
        generate_sensor_data(data);


        /*
         * Determine machine status.
         */
        check_status(data);


        /*
         * Display sensor information.
         */
        printf("\n============================================\n");

        printf("Machine: %s\n",
               data->machine_name);

        printf("--------------------------------------------\n");

        printf("Temperature : %d C\n",
               data->temperature);

        printf("Vibration   : %d mm/s\n",
               data->vibration);

        printf("Pressure    : %d PSI\n",
               data->pressure);

        printf("Status      : %s\n",
               data->status);

        printf("--------------------------------------------\n");


        /*
         * Display fault alerts.
         */
        if (data->status[0] == 'C')
        {
            printf(
                "CRITICAL ALERT! %s requires attention.\n",
                data->machine_name
            );
        }
        else if (data->status[0] == 'W')
        {
            printf(
                "WARNING! %s condition is abnormal.\n",
                data->machine_name
            );
        }


        /*
         * Store sensor data in SQLite.
         */
        save_data(db, data);


        /*
         * Store event in log file.
         */
        log_event(data);


        /*
         * Publish sensor data through MQTT
         * simulation.
         */
        publish_sensor_data(data);


        /*
         * Unlock shared sensor data.
         */
        pthread_mutex_unlock(&sensor_mutex);


        /*
         * Wait 3 seconds before the next
         * sensor reading.
         */
        sleep(3);
    }


    return NULL;
}