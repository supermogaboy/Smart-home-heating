#pragma once
#include <Arduino.h>
#include <esp_now.h>

extern bool espnowReady;
extern esp_now_peer_info_t peerInfo;
extern uint8_t broadcastAddress[6];

typedef struct __attribute__((packed)) SensorPacket {
    uint8_t  protocolVersion;   // For future changes to the packet format
    uint8_t  nodeID;            // Unique ID for each device (room sensor, furnace sensor, etc.)
    uint8_t  sensorType;        // 1 = temp, 2 = furnace, 

    float    temperature;       // C°
    float    humidity;          // %
    float    outdoorTemp;       // Only used by outdoor sensor (others send 0)

    uint32_t timestamp;         // UNIX time (seconds since 1970)

    uint8_t  heatingState;      // 0 = off, 1 = on/60% 2=on/100%(for furnace sensor)

    
}datapack;
void data_collect(datapack *collection);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);