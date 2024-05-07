// Arduino UNO
// coded 7/5/2024 by Paolo C.

#define goPin 9
#define rel_1 10 //+12 V
#define rel_2 11 // switch cable A
#define rel_3 12 // switch cable B

#define TRAVEL_LEFT 3500
#define PAUSE_LEFT 1500
#define TRAVEL_RIGHT 3500
#define PAUSE_RIGHT 1500

void setup() {

  digitalWrite(rel_1, HIGH);
  digitalWrite(rel_2, HIGH);
  digitalWrite(rel_3, HIGH);

  delay(50);

  pinMode(goPin, INPUT);
  pinMode(rel_1, OUTPUT);
  pinMode(rel_2, OUTPUT);
  pinMode(rel_3, OUTPUT);

  delay(50);
}

bool goS = false;
unsigned long goT = 0;

bool goStart = false;
bool enRead = true;

void loop() {

  // read go signal

  if (!goS) {
    if ((millis() - goT) > 200) {
      if (digitalRead(goPin) == 0) {
        goS = true;
        goStart = true;
        goT = millis();
      }
    }
  } else {
    if ((millis() - goT) > 200) {
      if (digitalRead(goPin) == 1) {
        goS = false;
        goT = millis();
      }
    }
  }

  // act
  if (goStart) {
    goStart = false;

    goLeft(TRAVEL_LEFT / 2);
    stop(PAUSE_LEFT);

    for (int i = 0; i < 4; i++) {
      goRight(TRAVEL_RIGHT);
      stop(PAUSE_RIGHT);
      goLeft(TRAVEL_LEFT);
      stop(PAUSE_LEFT);
    }

    goRight(TRAVEL_RIGHT / 2);
  } else {
    idle();
  }

}

void idle() {
  digitalWrite(rel_3, HIGH);
  digitalWrite(rel_2, HIGH);
  digitalWrite(rel_1, HIGH);
}

void stop(int tt) {
  digitalWrite(rel_1, HIGH);
  delay(tt);
}

void goLeft(int tt) {
  digitalWrite(rel_3, HIGH);
  digitalWrite(rel_2, HIGH);
  delay(100);
  digitalWrite(rel_1, LOW);
  delay(tt-100);
}

void goRight(float tt) {
  digitalWrite(rel_3, LOW);
  digitalWrite(rel_2, LOW);
  delay(100);
  digitalWrite(rel_1, LOW);
  delay(tt-100);
}

