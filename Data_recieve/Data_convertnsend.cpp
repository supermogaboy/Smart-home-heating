#include <WiFi.h>
#include <PubSubClient.h>
#include "data_recieve.h"  

extern const char* WIFI_SSID;
extern const char* WIFI_PASS;
extern const char* MQTT_SERVER;
extern const int   MQTT_PORT;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

static void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
}

static void ensureMQTT() {
  if (mqttClient.connected()) return;

  while (!mqttClient.connected()) {
    String clientId = "espnow-relay-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (!mqttClient.connect(clientId.c_str())) {
      delay(1000);
    }
  }
}

void buildCSV(const datapack &p, char *buffer, size_t size) {
  uint32_t rxTimeSec = millis() / 1000;

  // rxTimeSec,nodeID,sensorType,temp,hum,outdoor,timestamp,heatingState,protocolVersion
  snprintf(buffer, size,
    "%lu,%u,%u,%.2f,%.2f,%.2f,%lu,%u,%u",
    (unsigned long)rxTimeSec,
    (unsigned)p.nodeID,
    (unsigned)p.sensorType,
    p.temperature,
    p.humidity,
    p.outdoorTemp,
    (unsigned long)p.timestamp,
    (unsigned)p.heatingState,
    (unsigned)p.protocolVersion
  );
}

void publishCSV(const datapack &p) {
  char payload[160];
  buildCSV(p, payload, sizeof(payload));
  mqttClient.publish("home/sensors/csv", payload);
}


void mqttBegin() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  ensureWiFi();
  ensureMQTT();
}

void mqttPump() {
  ensureWiFi();
  ensureMQTT();
  mqttClient.loop();

  datapack p;//empty queue and push
  while (rxQueue && xQueueReceive(rxQueue, &p, 0) == pdTRUE) {
    publishCSV(p);
  }
}
