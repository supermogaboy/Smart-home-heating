#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>

#include "data_send.h"   // brings in datapack, data_collect, OnDataSent
const uint8_t PIN_I2C_SDA = SDA;  // D4
const uint8_t PIN_I2C_SCL = SCL;  // D5
extern AHT20 aht20;      // declared in Data_collect

void setup() { // indoor temperature sensor script 
  Serial.begin(115200);
  delay(100);
  analogReadResolution(12);
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  if (!aht20.begin()) {
    Serial.println("AHT20 not detected. Please check wiring. Freezing.");
    while (true) { delay(100); }
  }

  // set device as wifi station
  WiFi.mode(WIFI_STA);

  // start ESP-NOW protocol
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    espnowReady = false;
    return;
  }

  // Register send callback for fails
  esp_now_register_send_cb(OnDataSent);
  
  // register peer
  memset(&peerInfo, 0, sizeof(peerInfo));//clear peerInfo struct for clean config
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);//add peer mac adress to peer info
  peerInfo.channel = 0;//set current wifi channel as peer info channel
  peerInfo.encrypt = false;// no need to encrypt data also faster
  
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {//if adding peer fails exit 
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("starting loop");
}

void loop() {
  datapack collected;//initialize data packet
  data_collect(&collected);//collect data

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress,(uint8_t*)&collected, sizeof(collected));//send collected bits
   
  if (result == ESP_OK) {
    Serial.print("Sent OK | ");
    Serial.print("nodeID="); Serial.print(collected.nodeID);
    Serial.print(", sensorType="); Serial.print(collected.sensorType);
    Serial.print(", temp="); Serial.print(collected.temperature, 2); Serial.print("C");
    Serial.print(", hum="); Serial.print(collected.humidity, 2); Serial.print("%");
    Serial.print(", outdoorTemp="); Serial.print(collected.outdoorTemp, 2);
    Serial.print(", timestamp="); Serial.print(collected.timestamp);
    Serial.print(", heatingState="); Serial.print(collected.heatingState);
  } else {
    Serial.println("Error sending the data");//output data for debugging 
  }

  delay(2000);  //temp fix until i am ready to add deep sleep 
}
