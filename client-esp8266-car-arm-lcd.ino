#include <ESP8266WiFi.h>

const char* serverHost = "192.168.1.245"; // Server IP Address
const int httpPort = 80;

WiFiClient client;

char ssid[32] = {0};
char password[64] = {0};
bool wifiSetupDone = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // Wait for Serial to initialize
  }
}

void loop() {
  static bool isReadingSSID = true;
  static int ssidLength = 0;
  static int passwordLength = 0;

  if (!wifiSetupDone) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '`') { // End of input
        if (isReadingSSID) {
          ssid[ssidLength] = '\0'; // Null-terminate the SSID string
          isReadingSSID = false; // Start reading password next
        } else {
          password[passwordLength] = '\0'; // Null-terminate the password string
          wifiSetupDone = true;
          connectToWiFi();
        }
      } else {
        if (isReadingSSID) {
          ssid[ssidLength++] = c; // Append character to SSID and increment length
        } else {
          password[passwordLength++] = c; // Append character to password and increment length
        }
      }
    }
  } else {
    // WiFi is set up, handle other tasks
    if (WiFi.status() != WL_CONNECTED) {
      reconnectToWiFi();
    }

    // Your regular loop code goes here
    if (Serial.available()) {
      String commandPath = Serial.readStringUntil('\n'); // Read the command from Arduino
      sendCommand(commandPath); // Send the command to the server
    }
  }
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnectToWiFi() {
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void sendCommand(String commandPath) {
  if (client.connect(serverHost, httpPort)) {
    client.print(String("GET ") + commandPath + " HTTP/1.1\r\n" +
                 "Host: " + serverHost + "\r\n" +
                 "Connection: close\r\n\r\n");
    while (!client.available()) {
      delay(1);
    }
    client.stop();
  }
}
