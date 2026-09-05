#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>
#include <sqlite3.h>

#include "sensor.h"

typedef struct
{
    SensorData *data;
    sqlite3 *db;

} MonitorArgs;

/* Shared mutex for sensor data */
extern pthread_mutex_t sensor_mutex;

void *monitor_machine(void *arg);

#endif