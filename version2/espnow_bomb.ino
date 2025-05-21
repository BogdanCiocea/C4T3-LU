#include <ESP8266WiFi.h>
#include <espnow.h>

void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (len > 0) {
    char command = incomingData[0];
    Serial.println(command);

    switch (command) {
      case 'Z': Serial.println("Z"); break;
      default: Serial.println("Unknown command"); break;
    }
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("ESP-NOW receiver ready");
  Serial.println(WiFi.macAddress());
}

void loop() {
}
