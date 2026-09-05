#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "logger.h"

/* Mutex protects the event log file */
pthread_mutex_t logger_mutex = PTHREAD_MUTEX_INITIALIZER;

void log_event(SensorData *data)
{
    pthread_mutex_lock(&logger_mutex);

    FILE *file = fopen("logs/event_log.txt", "a");

    if (file == NULL)
    {
        printf("Error opening event log file.\n");

        pthread_mutex_unlock(&logger_mutex);
        return;
    }

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    char timestamp[50];

    strftime(
        timestamp,
        sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S",
        local_time
    );

    fprintf(
        file,
        "%s | Machine: %s | Temperature: %d C | Vibration: %d mm/s | Pressure: %d PSI | Status: %s\n",
        timestamp,
        data->machine_name,
        data->temperature,
        data->vibration,
        data->pressure,
        data->status
    );

    fclose(file);

    pthread_mutex_unlock(&logger_mutex);
}