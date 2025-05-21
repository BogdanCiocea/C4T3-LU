#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t serverMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t bombMac[] = {0xDC, 0x4F, 0x22, 0x2C, 0x9B, 0x13};

void onDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  if (esp_now_add_peer(serverMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP-NOW client ready");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd != 'Z') {
      esp_now_send(serverMac, (uint8_t *)&cmd, sizeof(cmd));
    } else {
      esp_now_send(bombMac, (uint8_t *)&cmd, 1);
    }

    Serial.print("Sent command: ");
    Serial.println(cmd);
  }
}
