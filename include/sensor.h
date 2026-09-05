#ifndef SENSOR_H
#define SENSOR_H

typedef struct
{
    char machine_name[30];

    int temperature;
    int vibration;
    int pressure;

    char status[20];

} SensorData;

void generate_sensor_data(SensorData *data);

#endif