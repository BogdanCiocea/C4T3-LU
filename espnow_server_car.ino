#include <ESP8266WiFi.h>
#include <espnow.h>

void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (len > 0) {
    char command = incomingData[0];

    switch (command) {
      case 'F': Serial.println("F"); Serial.flush(); break;
      case 'B': Serial.println("B"); Serial.flush(); break;
      case 'L': Serial.println("L"); Serial.flush(); break;
      case 'R': Serial.println("R"); Serial.flush(); break;
      // case 'S': Serial.println("S"); break;
      case 'V': Serial.println("V"); Serial.flush(); break;
      case 'U': Serial.println("U"); Serial.flush(); break;
      case 'Y': Serial.println("Y"); Serial.flush(); break;
      case 'D': Serial.println("D"); Serial.flush(); break;
      case 'C': Serial.println("C"); Serial.flush(); break;
      default: break;
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
