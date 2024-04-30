#define goPin 6
#define rel_1 10
#define rel_2 11
#define rel_3 12

#define TRAVEL_LEFT 2000
#define PAUSE_LEFT 500
#define TRAVEL_RIGHT 2000
#define PAUSE_RIGHT 500

void setup() {
  delay(1000);

  pinMode(goPin, INPUT);
  pinMode(rel_1, OUTPUT);
  pinMode(rel_2, OUTPUT);
  pinMode(rel_3, OUTPUT);

  delay(1000);
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

    for (int i = 0; i < 4; i++) {
      goLeft(TRAVEL_RIGHT);
      delay(PAUSE_RIGHT);
      goRight(TRAVEL_LEFT);
      delay(PAUSE_LEFT);
    }

    goRight(TRAVEL_RIGHT / 2);
  }
}
