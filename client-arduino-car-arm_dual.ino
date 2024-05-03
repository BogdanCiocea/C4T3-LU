#include "Arduino_SensorKit.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

const float MIN_THRESHOLD = 0.7;
const float MAX_THRESHOLD = 2.0;

const char keyboard[] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h',
                         'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C',
                         'V', 'B', 'N', 'M', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_',
                         '`'};

void show_keyboard(int level, int cursor, String characters) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(characters);

  int startIndex = level * 16;

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i == cursor) {
      lcd.print("*");
    } else {
      lcd.print(keyboard[startIndex + i]);
    }
  }
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

// this function will be at the setup and will help us connect to the wifi
// firstly it will send the bytes for the wifi name and then the password
// the charcters array will be displayed at the top row of the lcd display,
// and will show what i've wrote, it will concatenate the letters i've typed
void connect_to_the_wifi() {
  int level = 0, cursor = 0;
  String currentInput = ""; // Use a String to hold the input for easier concatenation
  int times = 0;
  while (true) { // Loop indefinitely until `btnSELECT` is pressed on the 'backtick' character
    show_keyboard(level, cursor, currentInput);
    int button = read_LCD_buttons();
    delay(50); // Simple debounce

    if (button == btnRIGHT && cursor < 15) {
      cursor++;
    } else if (button == btnLEFT && cursor > 0) {
      cursor--;
    } else if (button == btnUP && level > 0) {
      level--;
    } else if (button == btnDOWN && level < 4) { // Ensure level doesn't exceed bounds
      level++;
    } else if (button == btnSELECT && keyboard[level * 16 + cursor] != '`') {
      currentInput += keyboard[level * 16 + cursor]; // Concatenate the selected character
      Serial.println(keyboard[level * 16 + cursor]); // Print the selected character to the serial monitor
    } else if (button == btnSELECT && keyboard[level * 16 + cursor] == '`') {
      times++;
      if (times == 2) {
        break;
      } else {
        currentInput = "";
        continue;
      }
    }
    delay(100); // Delay to allow for button press processing
  }
}

bool arm = false;
bool keys = false;

void check_mode() {
  lcd.clear();

  lcd.print("ARM     KEYS");
  while (true) {
    int buttons = read_LCD_buttons();
    delay(10);
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
  connect_to_the_wifi();
  Accelerometer.begin(); // Initialize the Accelerometer
  check_mode();
  lcd.clear();
  lcd.print("Connecting...");
}

void loop() {
  lcd.clear();
  // Read accelerometer values
  if (arm) {
    float x = Accelerometer.readX();
    float y = Accelerometer.readY();
    float z = Accelerometer.readZ();
    // Detect diagonal movements
    if (x <= -MIN_THRESHOLD + 0.48 && y <= -MIN_THRESHOLD) {
      Serial.println("/button/uright");
      lcd.setCursor(4, 0);
      lcd.print("UP-RIGHT");
    } else if (x >= MIN_THRESHOLD - 0.5 && y <= -MIN_THRESHOLD) {
      Serial.println("/button/uleft");
      lcd.setCursor(4, 0);
      lcd.print("UP-LEFT");
    } else if (x <= -MIN_THRESHOLD + 0.8 && y >= MIN_THRESHOLD) {
      Serial.println("/button/dright");
      lcd.setCursor(4, 0);
      lcd.print("DOWN-RIGHT");
    } else if (x >= MIN_THRESHOLD - 0.15 && y >= MIN_THRESHOLD) {
      Serial.println("/button/dleft");
      lcd.setCursor(4, 0);
      lcd.print("DOWN-LEFT");
    } else if (x <= -MIN_THRESHOLD + 0.2 && y > -MIN_THRESHOLD) {
      Serial.println("/button/right"); // Tilted significantly to the right
      lcd.setCursor(5, 0);
      lcd.print("RIGHT");
    } else if (x >= MIN_THRESHOLD + 0.1) {
      Serial.println("/button/left"); // Tilted significantly to the left
      lcd.setCursor(5, 0);
      lcd.print("LEFT");
    } else if (y <= -MIN_THRESHOLD) {
      Serial.println("/button/up"); // Tilted significantly upwards
      lcd.setCursor(2, 0);
      lcd.print("CHAAAAARGE!!");
    } else if (y >= MIN_THRESHOLD) {
      Serial.println("/button/down"); // Tilted significantly downwards
      lcd.setCursor(3, 0);
      lcd.print("BACKWARDS");
    }

    delay(100); // Adjust delay as needed for your application
  } else if (keys) {
    int buttonState = read_LCD_buttons();
    if (buttonState != btnNONE) {
      if (buttonState == btnUP) {
        Serial.println("/button/up");
        lcd.setCursor(2, 0);
        lcd.print("CHAAAAARGE!!");
        buttonState = read_LCD_buttons();
        delay(100);
      } else if (buttonState == btnRIGHT) {
        Serial.println("/button/right");
        lcd.setCursor(5, 0);
        lcd.print("RIGHT");
        buttonState = read_LCD_buttons();
        delay(100);
      } else if (buttonState == btnLEFT) {
        Serial.println("/button/left");
        lcd.setCursor(5, 0);
        lcd.print("LEFT");
        buttonState = read_LCD_buttons();
        delay(100);
      } else if (buttonState == btnDOWN) {
        Serial.println("/button/down");
        lcd.setCursor(3, 0);
        lcd.print("BACKWARDS");
        buttonState = read_LCD_buttons();
        delay(100);
      } else if (buttonState == btnSELECT) {
        Serial.println("/button/select");
        lcd.setCursor(5, 0);
        lcd.print("DANCE");
        buttonState = read_LCD_buttons();
        delay(3000);
      } else {
        Serial.println("/button/stop");
      }
    }
  }
}
