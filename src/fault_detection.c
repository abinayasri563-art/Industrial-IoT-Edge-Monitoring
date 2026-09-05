#include <string.h>
#include "fault_detection.h"

#define TEMP_WARNING 90
#define TEMP_CRITICAL 100

#define VIB_WARNING 7
#define VIB_CRITICAL 9

#define PRESS_WARNING 110
#define PRESS_CRITICAL 115

void check_status(SensorData *data)
{
    if (data->temperature > TEMP_CRITICAL ||
        data->vibration > VIB_CRITICAL ||
        data->pressure > PRESS_CRITICAL)
    {
        strcpy(data->status, "CRITICAL");
    }
    else if (data->temperature > TEMP_WARNING ||
             data->vibration > VIB_WARNING ||
             data->pressure > PRESS_WARNING)
    {
        strcpy(data->status, "WARNING");
    }
    else
    {
        strcpy(data->status, "NORMAL");
    }
}