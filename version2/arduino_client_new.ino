#include "Arduino_SensorKit.h"
#include <LiquidCrystal.h>
#include <neotimer.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define COUNT 3
const float MIN_THRESHOLD = 0.7;
const unsigned long long TIMER_DELAY = 100;

int select_count = 0;

Neotimer sensorTimer(10);
Neotimer actionTimer(75);
Neotimer keysTimer(100);

bool arm = false, keys = false;
int lastKeyState = btnNONE;

int read_LCD_buttons() {
  int v = analogRead(0);
  if (v < 50)   return btnRIGHT;
  if (v < 250)  return btnUP;
  if (v < 450)  return btnDOWN;
  if (v < 650)  return btnLEFT;
  if (v < 850)  return btnSELECT;
  return btnNONE;
}

void check_mode() {
  lcd.clear();
  lcd.print("ARM     KEYS");
  sensorTimer.start();
  while (true) {
    if (sensorTimer.done()) {
      int b = read_LCD_buttons();
      sensorTimer.start();
      if (b == btnLEFT)  { arm = true;  break; }
      if (b == btnRIGHT) { keys = true; break; }
    }
  }
  lcd.clear();
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  Accelerometer.begin();
  check_mode();
  lcd.print("Connecting...");
  
  sensorTimer.start();
  actionTimer.start();
  keysTimer.start();
}

void loop() {
  lcd.clear();

  if (arm) {
    float x, y, z;
    if (sensorTimer.done()) {
      x = Accelerometer.readX();
      y = Accelerometer.readY();
      z = Accelerometer.readZ();
      sensorTimer.start();
    }

    if (sensorTimer.done()) {
      int bs = read_LCD_buttons();
      sensorTimer.start();
      if (bs == btnSELECT)
        select_count++;
    }

    if (select_count >= COUNT) {
      select_count = 0;
      Serial.println("Z");
    } else if (actionTimer.done()) {
      actionTimer.start();

      if (x <= -MIN_THRESHOLD && y <= -MIN_THRESHOLD) {
        Serial.println("Y"); lcd.setCursor(4,0); lcd.print("UP-RIGHT");
      } else if (x >=  MIN_THRESHOLD && y <= -MIN_THRESHOLD) {
        Serial.println("U"); lcd.setCursor(4,0); lcd.print("UP-LEFT");
      } else if (x <= -MIN_THRESHOLD && y >=  MIN_THRESHOLD) {
        Serial.println("V"); lcd.setCursor(4,0); lcd.print("DOWN-RIGHT");
      } else if (x >=  MIN_THRESHOLD && y >=  MIN_THRESHOLD) {
        Serial.println("C"); lcd.setCursor(4,0); lcd.print("DOWN-LEFT");
      } else if (x <= -MIN_THRESHOLD) {
        Serial.println("R"); lcd.setCursor(5,0); lcd.print("RIGHT");
      } else if (x >=  MIN_THRESHOLD) {
        Serial.println("L"); lcd.setCursor(5,0); lcd.print("LEFT");
      } else if (y <= -MIN_THRESHOLD) {
        Serial.println("F"); lcd.setCursor(2,0); lcd.print("CHAAAAARGE!!");
      } else if (y >=  MIN_THRESHOLD) {
        Serial.println("B"); lcd.setCursor(3,0); lcd.print("BACKWARDS");
      }
    }
  } else if (keys) {
    if (keysTimer.done()) {
      keysTimer.start();
      int bs = read_LCD_buttons();
        switch (bs) {
          case btnUP:
            Serial.println("F");
            lcd.setCursor(2,0); lcd.print("CHAAAAARGE!!");
            break;
          case btnRIGHT:
            Serial.println("R");
            lcd.setCursor(5,0); lcd.print("RIGHT");
            break;
          case btnLEFT:
            Serial.println("L");
            lcd.setCursor(5,0); lcd.print("LEFT");
            break;
          case btnDOWN:
            Serial.println("B");
            lcd.setCursor(3,0); lcd.print("BACKWARDS");
            break;
          case btnSELECT:
            Serial.println("Z");
            lcd.setCursor(3,0); lcd.print("Set bomb");
            select_count++;
            break;
      }
    }
  }
}
