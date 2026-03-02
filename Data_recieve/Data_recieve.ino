#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "data_recieve.h"   




QueueHandle_t rxQueue = nullptr;// define queue that handles packets

static volatile uint32_t droppedPackets = 0;//count packets

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len != (int)sizeof(datapack)) {
  droppedPackets++;
  return;//if input packet len does not match size of datapack ignore
  }

  datapack p;
  memcpy(&p, data, sizeof(p));
  p.timestamp = millis() / 1000;

  if (xQueueSend(rxQueue, &p, 0) != pdTRUE) {//if packet is not added to queue add to count
    droppedPackets++;
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);

  rxQueue = xQueueCreate(64, sizeof(datapack));
  if (rxQueue == NULL) {//check for queue failure
    Serial.println("ERROR: Queue creation failed!");
    while (true) {
      delay(1000); 
    }
  }
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  mqttBegin();
  Serial.println("ESP-NOW receiver ready. Queueing packets...");

}

void loop() {
  static uint32_t prev = 0;
  mqttPump();
  delay(1);
  if(millis()-prev>10000){
    Serial.printf("dropped %lu packets\n",droppedPackets);
    prev=millis();
  }
}