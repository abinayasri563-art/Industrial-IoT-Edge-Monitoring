#include <stdio.h>

#include "mqtt.h"


/*
 * MQTT publishing simulation.
 *
 * This function represents the MQTT publishing
 * layer of the Industrial IoT system.
 *
 * In a real deployment, this payload can be
 * published to an MQTT broker such as Mosquitto.
 */

void publish_sensor_data(SensorData *data)
{
    printf("\n");
    printf("[MQTT] Publishing sensor data...\n");

    printf(
        "[MQTT] Topic: industrial/%s/sensors\n",
        data->machine_name
    );

    printf(
        "[MQTT] Payload: "
        "{"
        "\"machine\":\"%s\","
        "\"temperature\":%d,"
        "\"vibration\":%d,"
        "\"pressure\":%d,"
        "\"status\":\"%s\""
        "}\n",
        data->machine_name,
        data->temperature,
        data->vibration,
        data->pressure,
        data->status
    );

    printf("[MQTT] Message published successfully.\n");
}