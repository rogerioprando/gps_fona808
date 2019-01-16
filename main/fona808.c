#include "fona808.h"

int FLAG_SEND = 0;
int onoff = 1;

void fona808_init_serial() {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

int send_data(const char* data)
{
    static const char *TX_FONA = "TX_FONA";
    esp_log_level_set(TX_FONA, ESP_LOG_INFO);
    ESP_LOGI(TX_FONA, "%s", data);

    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    
    return txBytes;
}

int send_and_check_reply(char *command, char * reply)
{
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    
    static  char *RX_FONA = "RX_FONA";
    esp_log_level_set(RX_FONA, ESP_LOG_INFO);
    printf("send_and_check_reply\n");
    send_data(command);
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);

    if(rxBytes > 0)
    {
        //ESP_LOGI(RX_FONA, "%s", data);
        ESP_LOG_BUFFER_HEXDUMP(RX_FONA, data, rxBytes, ESP_LOG_INFO);
        if(strncmp((const char*)data, reply, 16) == 0)
        {
            printf("strncmp ok %c\n", data[16]);
            free(data);
            return 0;
        }
    }

    free(data);
    return -1;

}

int fona808_toggle_gps(uint8_t onoff)
{
    
    static const char *RX_FONA = "RX_FONA";
    esp_log_level_set(RX_FONA, ESP_LOG_INFO);

    // check if already on
    send_data("AT+CGPSPWR?\r");
    uint8_t state = 0, r = 0;
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);

    if (rxBytes > 0)
    {
        data[rxBytes] = 0;
        //printf("***[recv]: %s***\n", data);
        //ESP_LOG_BUFFER_HEXDUMP(RX_FONA, data, rxBytes, ESP_LOG_INFO); 
        ESP_LOGI(RX_FONA, "%s", data);
        if(strncmp((const char*)data, "AT+CGPSPWR?\r\r\n+CGPSPWR: ", 23) == 0)
        {
            state = data[24] == '1' ? 1 : 0;
            if(state == onoff)
            {
                printf("GPS already %s\n", state == 1 ? "ON" : "OFF");
            }

        }
    }

    if(onoff && !state)     // turn on GPS
    {
        printf("turn on\n");
        if ((r = send_and_check_reply("AT+CGPSPWR=1\r", "AT+CGPSPWR=1\r\r\nOK")) == 0)
        {
            printf("GPS turned on\n");          
        }
        else
        {
            printf("Error\n");
        }

    }
    else if(!onoff && state)    // turn off GPS
    {
        printf("turn off\n");
        if ((r = send_and_check_reply("AT+CGPSPWR=0\r","AT+CGPSPWR=0\r\r\nOK")) == 0)
        {
            printf("GPS turned off\n");    
        }
        else
        {
            printf("Error\n");
        }
        
    }
    free(data);
    return 0;
}

int fona808_get_gps_status()
{
    
    static const char *RX_FONA = "RX_FONA";
    esp_log_level_set(RX_FONA, ESP_LOG_INFO);

    send_data("AT+CGPSSTATUS?\r");
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);

    if (rxBytes > 0)
    {
        data[rxBytes] = 0;
        ESP_LOG_BUFFER_HEXDUMP(RX_FONA, data, rxBytes, ESP_LOG_INFO);
        char *p = strstr((const char*) data, "+CGPSSTATUS: Location ");
        if (p == 0)
            return -1;
        p += 22;

        if(p[0] == 'U'){
            printf("GPS OFF\n");
            return 1;
        }
        if(p[0] == 'N'){
            printf("GPS NOT FIX\n");
            return 2;
        }    
        if(p[0] == '2'){
            printf("GPS 2D FIX\n");
            return 3;
        }
        if(p[0] == '3'){
            printf("GPS 3D FIX\n");
            return 4;
        }

    }

    free(data);
    return 0;
}
//fona808_parse_gps
int fona808_parse_gps(const char *buf, uint16_t blen, struct gps_fix_t *const gpsfix)
{
    //BUFF: +CGPSINF=0
	//+CGPSINF: 0,2730.476636,4830.745555,44.142296,20190116123339.000,125,10,0.000000,354.314758
    uint16_t cursor = 27;
    printf("data: %s\n", (const char *)(&buf[cursor]));
    gpsfix->latitude = coord_from_chars((const char *)(&buf[cursor]), 11);
    cursor += 12;
    gpsfix->longitude = coord_from_chars((const char *)(&buf[cursor]), 11);
    cursor += 12;
 	uint16_t altitude_len = 0;
	while (altitude_len <= 4 && buf[cursor + altitude_len] != '.')
		altitude_len++;
	if (altitude_len > cursor + 4) {
		fprintf(stderr, "Invalid altitude data at index %d\n", cursor);
		return -1;
	}
	gpsfix->altitude =
	    chars_to_number((const char *)(&buf[cursor]), altitude_len, 10);
	cursor += altitude_len;
	while (cursor < 128 && buf[cursor] != ',')
		cursor++;
	if (cursor > 127) {
		fprintf(stderr, "No valid data past altitude field");
		return -2;
	}
	cursor++;

	gpsfix->timestamp_ms = chars_to_number((const char *)(&buf[cursor]), 14, 10);
	//gpsfix->timestamp_ms = gettimestamp_from_datestr((const char *)&buf[cursor]);
	cursor += 19; // timestamp + ','
	cursor += 7;  // skip time-to-first-fix and number of satellites fields
	//printf("Cursor before speedlen: %lu\n", cursor);
	size_t speedlen = 0;
	while (buf[cursor + speedlen] != ',')
		speedlen++;

	const uint8_t *endspeed = ((const uint8_t *)&buf[cursor + speedlen]);
	gpsfix->speed = strtof((const char *)&buf[cursor], (char **)&endspeed);
	cursor += speedlen + 1;
	gpsfix->course = strtof((const char *)&buf[cursor], NULL);
	return cursor;
    return 0;
}

//fona808_get_gps_fix
int fona808_get_gps_fix(struct gps_fix_t *const gpsfix)
{

    // need at least a 2D fix
    if (fona808_get_gps_status() < 3)
        return -2;

    static const char *RX_FONA = "RX_FONA";
    esp_log_level_set(RX_FONA, ESP_LOG_INFO);
    
    send_data("AT+CGPSINF=0\r");

    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        
    if (rxBytes > 0)
    {
        data[rxBytes] = 0;
        //ESP_LOG_BUFFER_HEXDUMP(RX_FONA, data, rxBytes, ESP_LOG_INFO);
        fona808_parse_gps((const char*) data, strlen((char*) data), gpsfix);
        

    }
    //fona808_parse_gps((const char*) data, strlen((char*)data), struct gps_fix_t *const gpsfix);
    
    free(data);
    return 0;
}





void showGPS(struct gps_fix_t *const gps) {
	printf("[SHOW GPS] enter\n");
    printf("\tlabel: %s\n", gps->label);
    printf("\tdevice: %s\n", gps->device_id);
    printf("\tlatitude: %lf\n", gps->latitude);
    printf("\tlongitude: %lf\n", gps->longitude);
    printf("\taltitude: %d\n", gps->altitude);
    printf("\tspeed: %f\n", gps->speed);
    printf("\tcourse: %f\n", gps->course);
}