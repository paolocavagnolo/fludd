/*
ARDUINO DUE
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(12);  // attaches the servo on pin 9 to the servo object
  pinMode(A1, INPUT_PULLUP);
  //while(true){};
}

void loop() {
  if (!digitalRead(A1)) {
    for (pos = 0; pos <= 85; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(80);                       // waits 15ms for the servo to reach the position
    }
    delay(2000);
    for (pos = 85; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(80);                       // waits 15ms for the servo to reach the position
    }
    delay(2000);
  }
  
}
