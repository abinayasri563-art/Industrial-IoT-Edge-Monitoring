#include <stdlib.h>
#include <string.h>

#include "sensor.h"

void generate_sensor_data(SensorData *data)
{
    data->temperature = 60 + rand() % 51;
    data->vibration = 1 + rand() % 10;
    data->pressure = 80 + rand() % 41;
}