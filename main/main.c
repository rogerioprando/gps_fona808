#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "utils.h"
#include "fona808.c"


void app_main()
{
    fona808_init_serial();
    struct gps_fix_t gpsfix      = {0};
    while(1)
    {   
        uint8_t r;
        while ((r = fona808_toggle_gps(ON)) == 0)
        {
            int x = 1;
            while(x != 0){
                x = fona808_get_gps_fix(&gpsfix);                
                vTaskDelay(500);
            }

            printf("delay\n");
            //vTaskDelay(100);
            printf("\tlatitude: %lf\n", gpsfix.latitude);
            printf("\tlatitude: %lf\n", gpsfix.longitude);
            printf("\taltitude: %d\n", gpsfix.altitude);
            printf("\thora: %llu\n", gpsfix.timestamp_ms);
            printf("\tvelocidade: %lf\n", gpsfix.speed);
            printf("\tdireção: %lf\n", gpsfix.course);
                  
        } 
    }

}
