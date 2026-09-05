#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <pthread.h>

#include "sensor.h"

int initialize_database(sqlite3 **db);

void save_data(sqlite3 *db, SensorData *data);

void close_database(sqlite3 **db);

#endif