#include <ESP8266WiFi.h>

#define BLYNK_TEMPLATE_ID "TMPL42M__C21Z"
#define BLYNK_TEMPLATE_NAME "masina"
#define BLYNK_AUTH_TOKEN "fLRewwUCIbJ__c5jeWEB4YenWPRlCRN5"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>

char ssid[] = "TP-Link_FE5C";        // Your WiFi SSID
char pass[] = "99445300";    // Your WiFi password

#define DELAY 60

bool V0Pressed = false;
bool V1Pressed = false;
bool V2Pressed = false;
bool V3Pressed = false;
bool V4Pressed = false;

void setup() {
  Serial.begin(9600);  // Start serial communication at 115200 baud rate
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void sendCommandToArduino(String command) {
  Serial.println(command);
}

BLYNK_WRITE(V0) {
  V0Pressed = param.asInt() == 1;
}

BLYNK_WRITE(V1) {
  V1Pressed = param.asInt() == 1;
}

BLYNK_WRITE(V2) {
  V2Pressed = param.asInt() == 1;
}

BLYNK_WRITE(V3) {
  V3Pressed = param.asInt() == 1;
}

BLYNK_WRITE(V4) {
  V4Pressed = param.asInt() == 1;
}

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10; // Time between commands in milliseconds

void loop() {
  Blynk.run();
  unsigned long currentTime = millis();

  if (currentTime - lastSendTime >= sendInterval) {
    if (V0Pressed) {
      sendCommandToArduino("F");
      delay(DELAY);
    } else if (V1Pressed) {
      sendCommandToArduino("B");
      delay(DELAY);
    } else if (V2Pressed) {
      sendCommandToArduino("L");
      delay(DELAY - 5);
    } else if (V3Pressed) {
      sendCommandToArduino("R");
      delay(DELAY - 5);
    } else if (V4Pressed) {
      sendCommandToArduino("D");
      delay(DELAY*10);
    } else {
      sendCommandToArduino("S");
    }
    lastSendTime = currentTime; // Update the last send time
  }
}
