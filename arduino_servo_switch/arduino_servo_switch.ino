/**
 * Author: Yamada, Yasuhiro
 * File Name: arduino_servo_switch.ino
 **/

#include <stdio.h>
#include <Servo.h>

Servo myservo;
const int servoPin = 9;
int pos = 0;

const int magnetPin = 0;
int magnetValue = 0;

const int switchPin = 8;
int nowSwitchState = LOW;
int oldSwitchState = LOW;

void door_close() {
  myservo.attach( servoPin );
  for ( pos = 35; pos < 145; pos += 1 ) {
    myservo.write( pos );
    delay( 15 );
  }
  myservo.detach();
}

void door_open() {
  myservo.attach( servoPin );
  for ( pos = 145; pos > 35; pos -= 1 ) {
    myservo.write( pos );
    delay( 15 );
  }
  myservo.detach();
}

void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT);
  //initialize the position
  door_close();
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
                 door_close();
                 break;
        case 1:
                 door_open();
                 break;
        case 2:
                 /*
                  * response the sensor's data to the server
                  * Format: [magnetValue]
                  * 0 > magnetValue > 1023
                  */
                 sprintf(res, "[%d]", magnetValue);
                 Serial.println(res);
                 break;
    }
  }

  /* When the switch is pushed */
  if (nowSwitchState == HIGH && oldSwitchState == LOW) {
    door_open();
  }

  oldSwitchState = nowSwitchState;

}
