/*
 * save_data_on_flash_memory_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include <save_data_on_flash_memory_task.h>

void save_data_on_flash_memory_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();


    char flash_package[FLASH_PACKAGE_LENGTH];
    char status_package[STATUS_PACKAGE_LENGTH];
    static unsigned long current_position = 1024; // to jump over the MBR data in SD
    /*** mmc initialization ***/
    unsigned long cardSize = 0;
    unsigned char status = 1;
    unsigned int timeout = 0;

    while (status != 0)                       // if return in not NULL an error did occur and the
                                                // MMC/SD-card will be initialized again
      {
        status = mmcInit();
        timeout++;
        if (timeout == 150)                      // Try 50 times till error
        {
          //printf ("No MMC/SD-card found!! %x\n", status);
          break;
        }
      }

      while ((mmcPing() != MMC_SUCCESS));      // Wait till card is inserted

      // Read the Card Size from the CSD Register
      cardSize =  mmcReadCardSize();


    while(1)
    {
        //TODO: TASK ROUTINE
        //save in the begining of the memory the log_status: (packages counter, resets counter, etc).
        sprintf(status_package, "Package count: %lu \n", current_position/FLASH_PACKAGE_LENGTH - 2);
        mmcWriteBlock(512, STATUS_PACKAGE_LENGTH, (unsigned char *) status_package);

        //TODO: corrigir sistema para salvar na memoria. Deve seguir o seguinte esquema:
        /* A cada iteração, no cabeçalho do pacote deve conter as flags cujos sensores foram lidos desde a execução anterior da tarefa de salvar. 
         * Desta forma, se a flag não estiver setada, aquele byte não deve ser enviado, poupando o downlink. 
         * Porém, o pacote é salvo no tamanho total no cartão, mesmo que ocupe mais espaço, com os bytes zerados onde a informação será ignorada.
         */

        sprintf(flash_package, "\n<aaaa|mm|dd|hh|mm|ss|msms>: %s\n%s\n%s\n%s\n\n", eps_data, imu_data, ttc_data, msp_internal_data);
        mmcWriteBlock(current_position, FLASH_PACKAGE_LENGTH, (unsigned char *) flash_package);
//        mmcWriteSector(current_position, flash_package);
        current_position += FLASH_PACKAGE_LENGTH;
//        current_position +=  sizeof(flash_package);
        vTaskDelayUntil( (TickType_t *) &last_wake_time, SAVE_DATA_ON_FLASH_MEMORY_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
