// @Shifa Ayaz - 600001200015
// unicore - Task 1

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define core_0 0
#define core_1 1

// Define pin number the default is 13 but it is not recommended to use it one can use another in build LED on PIN 2
#define PIN 2
// TAG for our logger
static const char *TAG = "LED STATUS";

// Creating TaskHandle
TaskHandle_t hello_world;
TaskHandle_t blink_led;

// xTaskCreatePinnedToCore passes pvParameter although not used here
void blink_led_core_1(void *pvParameter)
{
    // Select GPIO pin
    gpio_pad_select_gpio(PIN);
    // Set the log level to INFO
    esp_log_level_set("LOG", ESP_LOG_INFO);
    // set PIN 2 as output
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    // Initialize the signal
    int isON = 0;

    while (true)
    { // Set the output as signal isON
        gpio_set_level(PIN, isON);
        // A delay of 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // LOG the value of signal
        ESP_LOGI(TAG, "Turning the LED %s! from core %d  and priority %d", isON == true ? "ON" : "OFF", xPortGetCoreID(), uxTaskPriorityGet(blink_led));
        // change the value of signal
        isON = !isON;
        // clear (or flush) the output buffer and move the buffered data to console
        fflush(stdout);
    }
}

// xTaskCreatePinnedToCore passes pvParameter although not used here

void hello_task_core_0(void *pvParameter)
{
    while (1)
    { // Print hello world with a delay
        printf("Hello world from core %d! and prority %d \n", xPortGetCoreID(), uxTaskPriorityGet(hello_world));
        vTaskDelay(1323 / portTICK_PERIOD_MS);
        // clear (or flush) the output buffer and move the buffered data to console
        fflush(stdout);
    }
}

void app_main()
{ // init nvs flash storage
    nvs_flash_init();
    // Create task1
    xTaskCreatePinnedToCore(&hello_task_core_0, "core1_task", 1024 * 4, NULL, 1, &hello_world, core_0);
    // Create task2
    xTaskCreatePinnedToCore(&blink_led_core_1, "core0_task", 1024 * 4, NULL, 2, &blink_led, core_0);
}
