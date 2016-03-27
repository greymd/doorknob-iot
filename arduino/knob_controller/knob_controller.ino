/**
   Author: Yamada, Yasuhiro
   File Name: knob_controller.ino
 **/

#include <stdio.h>
#include <Servo.h>

Servo myservo;
const int servoPin      = 9;
int pos                 = 0;
const int magnetPin     = 0;
int magnetValue         = 0;
const int switchPin     = 8;
int nowSwitchState      = LOW;
int oldSwitchState      = LOW;
const int baudrate      = 9600;
const int servoWaitTime = 15;

/**
   Please adjust those values.
   They may be depending on your physical condition.
 **/
const int openAngle  = 35;
const int closeAngle = 145;

void doorClose() {
  myservo.attach( servoPin );
  for ( pos = openAngle; pos < closeAngle; pos += 1 ) {
    myservo.write( pos );
    delay( servoWaitTime );
  }
  myservo.detach();
}

void doorOpen() {
  myservo.attach( servoPin );
  for ( pos = closeAngle; pos > openAngle; pos -= 1 ) {
    myservo.write( pos );
    delay( servoWaitTime );
  }
  myservo.detach();
}

void setup() {
  Serial.begin(baudrate);
  pinMode(switchPin, INPUT);
  //initialize the position
  doorClose();
}

void loop() {

  char res[128];
  magnetValue = analogRead(magnetPin);
  nowSwitchState = digitalRead(switchPin);

  if (Serial.available()) {
    char tmp = Serial.read();
    int action = tmp - '0';
    switch (action) {
      case 0:
        doorClose();
        break;
      case 1:
        doorOpen();
        break;
      case 2:
        /*
           response the sensor's data to the server
           Format: [magnetValue]
           0 > magnetValue > 1023
        */
        sprintf(res, "[%d]", magnetValue);
        Serial.println(res);
        break;
    }
  }

  /* When the switch is pushed */
  if (nowSwitchState == HIGH && oldSwitchState == LOW) {
    doorOpen();
  }

  oldSwitchState = nowSwitchState;

}
