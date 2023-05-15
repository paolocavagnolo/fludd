// Arduino UNO
// Caricato l'ultima modifica il 16/12/2022

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER,3,4); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(4000);
  stepper.setAcceleration(5500);
  stepper.moveTo(5500);
}

uint8_t C = 0;

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0) {
      if (C < 10) {
      stepper.moveTo(-stepper.currentPosition());
      delay(100);
      C++;
      }
      else {
        stepper.moveTo(0);
        if (stepper.distanceToGo() == 0) {
          C = 0;
          while(true);
          stepper.moveTo(1100);
        }
      }
    }

    stepper.run();
 
}
