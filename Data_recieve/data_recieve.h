#pragma once
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

extern QueueHandle_t rxQueue;
void mqttBegin();
void mqttPump();

typedef struct __attribute__((packed)) SensorPacket {
  uint8_t  protocolVersion;
  uint8_t  nodeID;
  uint8_t  sensorType;      // 1=temp, 2=furnace, etc.

  float    temperature;
  float    humidity;
  float    outdoorTemp;

  uint32_t timestamp;       // receiver can overwrite
  uint8_t  heatingState;    // 0/1 or percent if you want
} datapack;

 