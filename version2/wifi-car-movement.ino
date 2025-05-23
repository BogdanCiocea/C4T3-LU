#include <Servo.h>
#include <stdbool.h>
#include <Arduino_PMIC.h>
#include <neotimer.h>

#define servo_PIN      9
#define Trig           12
#define Echo           13

#define PWM1_PIN       5
#define PWM2_PIN       6
#define SHCP_PIN       2
#define EN_PIN         7
#define DATA_PIN       8
#define STCP_PIN       4

#define MINIMUM_DISTANCE 15
#define SERVO_ANGLE       50
#define DELAY            100

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

Servo myservo;

Neotimer motorTimer(DELAY);
bool motorActive = false;
const char myCommands[] = {'F', 'B', 'L', 'R', 'Y', 'U', 'V', 'C'};

void setup() {
  Serial.begin(9600);
  myservo.attach(servo_PIN);
  myservo.write(SERVO_ANGLE);

  pinMode(SHCP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(PWM2_PIN, OUTPUT);

  Serial.println("Motors are active");
}

void loop() {

  if (Serial.available()) {
    char cmd = (char)Serial.read();
    Serial.println(cmd);
    applyCommand(cmd);
    if (isValid(cmd)) {
      motorTimer.start();
      motorActive = true;
    }
  }

  if (motorActive && motorTimer.waiting()) {
    Motor(Stop, 0);
    motorActive = false;
  }
}

bool isValid(char command) {
  for (char c : myCommands) {
    if (command == c)
      return true;
  }

  return false;
}

void applyCommand(char command) {
  switch (command) {
    case 'F':  Motor(Forward, 255); break;
    case 'B':  Motor(Backward,255); break;
    case 'L':  Motor(Contrarotate, 255); break;
    case 'R':  Motor(Clockwise, 255); break;
    case 'Y':  Motor(Top_Right, 255); break;
    case 'U':  Motor(Top_Left, 255); break;
    case 'V':  Motor(Bottom_Right, 255); break;
    case 'C':  Motor(Bottom_Left, 255); break;
    default:   Motor(Stop, 0); break;
  }
}

void Motor(int Dir, int Speed) {
  digitalWrite(EN_PIN, LOW);
  analogWrite(PWM1_PIN, Speed);
  analogWrite(PWM2_PIN, Speed);

  digitalWrite(STCP_PIN, LOW);
  shiftOut(DATA_PIN, SHCP_PIN, MSBFIRST, Dir);
  digitalWrite(STCP_PIN, HIGH);
}
