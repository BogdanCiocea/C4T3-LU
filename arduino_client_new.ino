#include "Arduino_SensorKit.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define forever while(1)

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define COUNT 3

const float MIN_THRESHOLD = 0.7;
const float MAX_THRESHOLD = 2.0;

int select_count = 0;

void my_delay(int time) {
  int time_now = millis();

  while(millis() < time_now + time);
}

int read_LCD_buttons() {
  int adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;
  return btnNONE;
}

bool arm = false;
bool keys = false;

void check_mode() {
  lcd.clear();

  lcd.print("ARM     KEYS");
  while (true) {
    int buttons = read_LCD_buttons();
    delay(10);
    // my_delay(10);
    if (buttons == btnLEFT) {
      arm = true;
      break;
    } else if (buttons == btnRIGHT) {
      keys = true;
      break;
    }
  }

  lcd.clear();
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  /*
   * Initialize the Accelerometer
   */
  Accelerometer.begin();
  check_mode();
  lcd.clear();
  lcd.print("Connecting...");
}

void loop() {
  lcd.clear();

  /*
   * ARM MOVEMENTS
   */
  if (arm) {
    /*
     * Read accelerometer values
     */
    float x = Accelerometer.readX();
    delay(10);
    // my_delay(10);
    float y = Accelerometer.readY();
    delay(10);
    // my_delay(10);
    float z = Accelerometer.readZ();
    delay(10);
    // my_delay(10);

    int buttonState = read_LCD_buttons();
    delay(10);
    // my_delay(10);

    if (buttonState == btnSELECT) {
      select_count++;
    }

    if (select_count == COUNT) {
      select_count = 0;
      Serial.println("Z");
    } else {
      /*
       * Detect movements
       */
      if (x <= -MIN_THRESHOLD + 0.48 && y <= -MIN_THRESHOLD) {
        Serial.println("Y");
        lcd.setCursor(4, 0);
        lcd.print("UP-RIGHT");
      } else if (x >= MIN_THRESHOLD - 0.5 && y <= -MIN_THRESHOLD) {
        Serial.println("U");
        lcd.setCursor(4, 0);
        lcd.print("UP-LEFT");
      } else if (x <= -MIN_THRESHOLD + 0.8 && y >= MIN_THRESHOLD) {
        Serial.println("V");
        lcd.setCursor(4, 0);
        lcd.print("DOWN-RIGHT");
      } else if (x >= MIN_THRESHOLD - 0.15 && y >= MIN_THRESHOLD) {
        Serial.println("C");
        lcd.setCursor(4, 0);
        lcd.print("DOWN-LEFT");
      } else if (x <= -MIN_THRESHOLD + 0.2 && y > -MIN_THRESHOLD) {
        Serial.println("R");
        lcd.setCursor(5, 0);
        lcd.print("RIGHT");
      } else if (x >= MIN_THRESHOLD + 0.1) {
        Serial.println("L");
        lcd.setCursor(5, 0);
        lcd.print("LEFT");
      } else if (y <= -MIN_THRESHOLD) {
        Serial.println("F");
        lcd.setCursor(2, 0);
        lcd.print("CHAAAAARGE!!");
      } else if (y >= MIN_THRESHOLD) {
        Serial.println("B");
        lcd.setCursor(3, 0);
        lcd.print("BACKWARDS");
      }
    }

    delay(75);
    // my_delay(75);
    // BUTTONS
  } else if (keys) {
    int buttonState = read_LCD_buttons();
    delay(10);
    // my_delay(10);
    /*
     * Same thing but with buttons now
     */
    if (buttonState != btnNONE) {
      if (buttonState == btnUP) {
        Serial.println("F");
        lcd.setCursor(2, 0);
        lcd.print("CHAAAAARGE!!");
        // Serial.flush();
        // buttonState = read_LCD_buttons();
        delay(100);
        // my_delay(100);
      } else if (buttonState == btnRIGHT) {
        Serial.println("R");
        lcd.setCursor(5, 0);
        lcd.print("RIGHT");
        // Serial.flush();
        // buttonState = read_LCD_buttons();
        delay(100);
        // my_delay(100);
      } else if (buttonState == btnLEFT) {
        Serial.println("L");
        lcd.setCursor(5, 0);
        lcd.print("LEFT");
        // Serial.flush();
        // buttonState = read_LCD_buttons();
        delay(100);
        // my_delay(100);
      } else if (buttonState == btnDOWN) {
        Serial.println("B");
        Serial.flush();
        lcd.setCursor(3, 0);
        lcd.print("BACKWARDS");
        // Serial.flush();
        // buttonState = read_LCD_buttons();
        delay(100);
        // my_delay(100);
      } else if (buttonState == btnSELECT) {
        Serial.println("Z");
        lcd.setCursor(3, 0);
        lcd.print("Set bomb");
        // Serial.flush();
        // buttonState = read_LCD_buttons();
        delay(100);
      }
    }
  }
}
