#include <stdio.h>
#include <string.h>

#include "../include/sensor.h"
#include "../include/fault_detection.h"

int main(void)
{
    SensorData data;

    strcpy(data.machine_name, "Test-Machine");

    /* Test 1: Normal condition */
    data.temperature = 70;
    data.vibration = 3;
    data.pressure = 90;

    check_status(&data);

    printf("Test 1 - Normal Condition\n");
    printf("Expected: NORMAL\n");
    printf("Actual  : %s\n\n", data.status);


    /* Test 2: Warning condition */
    data.temperature = 95;
    data.vibration = 5;
    data.pressure = 100;

    check_status(&data);

    printf("Test 2 - Warning Condition\n");
    printf("Expected: WARNING\n");
    printf("Actual  : %s\n\n", data.status);


    /* Test 3: Critical condition */
    data.temperature = 105;
    data.vibration = 9;
    data.pressure = 118;

    check_status(&data);

    printf("Test 3 - Critical Condition\n");
    printf("Expected: CRITICAL\n");
    printf("Actual  : %s\n\n", data.status);


    printf("All fault detection tests completed.\n");

    return 0;
}