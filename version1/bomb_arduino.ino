#include <stdbool.h>

#define DELAY 1000

const int led_pin = 8;
const int buzzer = 9;
bool isActivated;
int number_of_beeps = 10;

void setup() {
  Serial.begin(9600);
  isActivated = false;

  for (int i = 0; i < 2; i++) {
    tone(buzzer, 2000, 100);
    delay(100);
    noTone(buzzer);
    delay(100); 
  }

  pinMode(led_pin, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void explosion() {
  for (int i = 100; i <= 3000; i += 100) {
    // Play ascending tones
    tone(buzzer, i, 50);
    // Turn on the LED
    digitalWrite(led_pin, HIGH);
    delay(50);
    // Turn off the LED
    digitalWrite(led_pin, LOW);
    delay(50);
  }

  for (int i = 0; i <= 10; i++) {
    // Play ascending tones
    tone(buzzer, 5000, 50);
    // Turn on the LED
    digitalWrite(led_pin, HIGH);
    delay(50);

    // Turn off the LED
    digitalWrite(led_pin, LOW);
    delay(50);
  }

  // Stop the sound on the buzzer
  noTone(buzzer);
  Serial.print("BOOM!\n");
  delay(10);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();

    if (command == 'Z') {
      isActivated = true;
    }
  }

  if (isActivated && number_of_beeps > 0) {
    digitalWrite(led_pin, HIGH);
    tone(buzzer, 2000, 100);
    delay(500);
    digitalWrite(led_pin, LOW);
    noTone(buzzer);
    delay(500);
    number_of_beeps--;
  }

  if (isActivated && number_of_beeps == 0) {
    explosion();
    while(1);
  }
}
