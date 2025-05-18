#include <Servo.h>
#include <stdbool.h>
#include <Arduino_PMIC.h>
#define servo_PIN 9
Servo myservo;

const int Trig = 12;
const int Echo = 13;

#define DELAY 100

#define PWM1_PIN 5
#define PWM2_PIN 6
#define SHCP_PIN 2
#define EN_PIN 7
#define DATA_PIN 8
#define STCP_PIN 4
#define MINIMUM_DISTANCE 15
#define SERVO_ANGLE 50

const int Forward       = 92;                               // forward
const int Backward      = 163;                              // back
const int Turn_Left     = 149;                              // left translation
const int Turn_Right    = 106;                              // Right translation 
const int Top_Left      = 20;                               // Upper left mobile
const int Bottom_Left   = 129;                              // Lower left mobile
const int Top_Right     = 72;                               // Upper right mobile
const int Bottom_Right  = 34;                               // The lower right move
const int Stop          = 0;                                // stop
const int Contrarotate  = 172;                              // Counterclockwise rotation
const int Clockwise     = 83;

// void // my_delay(int time) {
//   int time_now = millis();

//   while(millis() < time_now + time);
// }

void setup() {
  /*
   * Match the baud rate to the ESP8266
   */
  Serial.begin(9600);

  myservo.attach(servo_PIN);
	myservo.write(SERVO_ANGLE);
	delay(100);
  // my_delay(100);

  pinMode(SHCP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(PWM2_PIN, OUTPUT);
}

bool is_legit(char command) {
  if (command == 'F' || command == 'B' || 
      command == 'L' || command == 'R' || 
      command == 'Y' || command == 'U' || 
      command == 'v' || command == 'c')
      return true;
  
  return false;
}

void loop() {
  if (Serial.available()) {
    char command = (char)Serial.read();
    controlMotor(command);
  }
}

/*
 * Function that takes a command and, based on that,
 * makes the robot move
 */
void controlMotor(char command) {
  switch (command) {
    case 'F':
      Motor(Forward, 255);
      // my_delay(DELAY);
      delay(DELAY);
      break;
    case 'B':
      Motor(Backward, 255);
      // my_delay(DELAY);
      delay(DELAY);
      break;
    case 'L':
      Motor(Contrarotate, 255);
      // my_delay(DELAY - 5);
      delay(DELAY - 5);
      break;
    case 'R':
      Motor(Clockwise, 255);
      delay(DELAY - 5);
      // my_delay(DELAY - 5);
      break;
    case 'S':
      // dance();
      break;
    case 'Y':
      Motor(Top_Right, 255);
      // my_delay(DELAY);
      delay(DELAY);
      break;
    case 'U':
      Motor(Top_Left, 255);
      delay(DELAY);
      // my_delay(DELAY);
      break;
    case 'V':
      Motor(Bottom_Right, 255);
      delay(DELAY);
      // my_delay(DELAY);
      break;
    case 'C':
      Motor(Bottom_Left, 255);
      delay(DELAY);
      // my_delay(DELAY);
      break;
    default:
      Motor(Stop, 0);
      break;
  }  
}

/*
 * Function that makes each motor move
 */
void Motor(int Dir, int Speed)
{
    digitalWrite(EN_PIN, LOW);
    analogWrite(PWM1_PIN, Speed);
    analogWrite(PWM2_PIN, Speed);

    digitalWrite(STCP_PIN, LOW);
    shiftOut(DATA_PIN, SHCP_PIN, MSBFIRST, Dir);
    digitalWrite(STCP_PIN, HIGH);
}
