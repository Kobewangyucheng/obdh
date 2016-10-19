/*
 * hibernate_task.c
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#include "../include/hibernate_task.h"

void prvHibernateTask( void *pvParameters )
{
    volatile TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        //TODO: TASK ROUTINE

        vTaskDelayUntil( &xLastWakeTime, HIBERNATE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
