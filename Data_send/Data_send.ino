#include <esp_now.h>
#include <WiFi.h>
#include "data_send.h"
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

 
