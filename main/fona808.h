#ifndef FONA808_H_
#define FONA808_H_

#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "utils.h"

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

#define ON 1
#define OFF 0

static const int RX_BUF_SIZE = 1024;

struct gps_fix_t {
    uint8_t *label;
    uint8_t *device_id;
    uint64_t timestamp_ms;
    float latitude;
    float longitude;
    uint32_t altitude;
    float speed;
    float course;
    float distance;
};

void fona808_init_serial();
int send_data(const char* data);
int send_and_check_reply(char *command, char * reply);
int fona808_toggle_gps(uint8_t onoff);
int fona808_get_gps_status();
int fona808_parse_gps(const char *buf, uint16_t blen, struct gps_fix_t *const gpsfix);
int fona808_get_gps_fix(struct gps_fix_t *const gpsfix);

void showGPS(struct gps_fix_t *const gps);

#endif /* FONA808_H_ */