deps_config := \
	/home/rogerio/esp/esp-idf/components/app_trace/Kconfig \
	/home/rogerio/esp/esp-idf/components/aws_iot/Kconfig \
	/home/rogerio/esp/esp-idf/components/bt/Kconfig \
	/home/rogerio/esp/esp-idf/components/driver/Kconfig \
	/home/rogerio/esp/esp-idf/components/esp32/Kconfig \
	/home/rogerio/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/rogerio/esp/esp-idf/components/esp_event/Kconfig \
	/home/rogerio/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/rogerio/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/rogerio/esp/esp-idf/components/ethernet/Kconfig \
	/home/rogerio/esp/esp-idf/components/fatfs/Kconfig \
	/home/rogerio/esp/esp-idf/components/freemodbus/Kconfig \
	/home/rogerio/esp/esp-idf/components/freertos/Kconfig \
	/home/rogerio/esp/esp-idf/components/heap/Kconfig \
	/home/rogerio/esp/esp-idf/components/libsodium/Kconfig \
	/home/rogerio/esp/esp-idf/components/log/Kconfig \
	/home/rogerio/esp/esp-idf/components/lwip/Kconfig \
	/home/rogerio/esp/esp-idf/components/mbedtls/Kconfig \
	/home/rogerio/esp/esp-idf/components/mdns/Kconfig \
	/home/rogerio/esp/esp-idf/components/mqtt/Kconfig \
	/home/rogerio/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/rogerio/esp/esp-idf/components/openssl/Kconfig \
	/home/rogerio/esp/esp-idf/components/pthread/Kconfig \
	/home/rogerio/esp/esp-idf/components/spi_flash/Kconfig \
	/home/rogerio/esp/esp-idf/components/spiffs/Kconfig \
	/home/rogerio/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/rogerio/esp/esp-idf/components/unity/Kconfig \
	/home/rogerio/esp/esp-idf/components/vfs/Kconfig \
	/home/rogerio/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/rogerio/esp/esp-idf/components/app_update/Kconfig.projbuild \
	/home/rogerio/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/rogerio/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/rogerio/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/rogerio/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
