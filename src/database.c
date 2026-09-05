#include <stdio.h>
#include <time.h>
#include <sqlite3.h>
#include <pthread.h>

#include "database.h"

/*
 * Mutex protects SQLite database access.
 */
pthread_mutex_t database_mutex = PTHREAD_MUTEX_INITIALIZER;


/*
 * Initialize SQLite database.
 */
int initialize_database(sqlite3 **db)
{
    int result;

    result = sqlite3_open(
        "industrial_iot.db",
        db
    );

    if (result != SQLITE_OK)
    {
        printf("Cannot open database.\n");

        return 0;
    }


    const char *sql =
        "CREATE TABLE IF NOT EXISTS sensor_data ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "machine_name TEXT NOT NULL,"
        "timestamp TEXT NOT NULL,"
        "temperature INTEGER,"
        "vibration INTEGER,"
        "pressure INTEGER,"
        "status TEXT"
        ");";


    char *error_message = NULL;


    result = sqlite3_exec(
        *db,
        sql,
        NULL,
        NULL,
        &error_message
    );


    if (result != SQLITE_OK)
    {
        printf(
            "Database error: %s\n",
            error_message
        );

        sqlite3_free(error_message);

        sqlite3_close(*db);

        *db = NULL;

        return 0;
    }


    return 1;
}


/*
 * Save sensor data using a prepared statement.
 */
void save_data(sqlite3 *db, SensorData *data)
{
    pthread_mutex_lock(&database_mutex);


    const char *sql =
        "INSERT INTO sensor_data "
        "(machine_name, timestamp, temperature, "
        "vibration, pressure, status) "
        "VALUES (?, ?, ?, ?, ?, ?);";


    sqlite3_stmt *statement = NULL;


    int result = sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        NULL
    );


    if (result != SQLITE_OK)
    {
        printf(
            "Failed to prepare database statement: %s\n",
            sqlite3_errmsg(db)
        );

        pthread_mutex_unlock(&database_mutex);

        return;
    }


    /*
     * Generate timestamp.
     */
    time_t now = time(NULL);

    struct tm *local_time = localtime(&now);

    char timestamp[50];


    strftime(
        timestamp,
        sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S",
        local_time
    );


    /*
     * Bind values.
     */
    sqlite3_bind_text(
        statement,
        1,
        data->machine_name,
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_text(
        statement,
        2,
        timestamp,
        -1,
        SQLITE_TRANSIENT
    );


    sqlite3_bind_int(
        statement,
        3,
        data->temperature
    );


    sqlite3_bind_int(
        statement,
        4,
        data->vibration
    );


    sqlite3_bind_int(
        statement,
        5,
        data->pressure
    );


    sqlite3_bind_text(
        statement,
        6,
        data->status,
        -1,
        SQLITE_TRANSIENT
    );


    /*
     * Execute statement.
     */
    result = sqlite3_step(statement);


    if (result != SQLITE_DONE)
    {
        printf(
            "Database insert failed: %s\n",
            sqlite3_errmsg(db)
        );
    }
    else
    {
        printf("Data saved to database.\n");
    }


    /*
     * Clean up statement.
     */
    sqlite3_finalize(statement);


    pthread_mutex_unlock(&database_mutex);
}


/*
 * Close SQLite database.
 */
void close_database(sqlite3 **db)
{
    if (*db != NULL)
    {
        sqlite3_close(*db);

        *db = NULL;
    }
}