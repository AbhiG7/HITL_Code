#include <Arduino.h>
#include <AccelStepper.h>
#include <CSV_Parser.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
long *timestep;
long *angles;

AccelStepper motor(AccelStepper::DRIVER,9,8);
CSV_Parser cp(/*format*/ "dd", /*has_header*/ true, /*delimiter*/ ',');

void setup() {
    Serial.begin(9600);
    delay(5000);

    Serial.print("Initializing SD card...");

    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");

      // don't do anything more:
      while (1);
    }
    Serial.println("card initialized.");

    // The line below (readSDfile) wouldn't work if SD.begin wasn't called before.
    // readSDfile can be used as conditional, it returns 'false' if the file does not exist.
    if (cp.readSDfile("/file.csv")) {
        timestep = (long*)cp["column_1"];
        angles = (long*)cp["column_2"];

        if (!timestep || !angles) {
          Serial.println("ERROR: At least 1 of the columns was not found, something went wrong.");
        }
    } else {
        Serial.println("ERROR: File called '/file.csv' does not exist...");
    }

    motor.setAcceleration(1000);
}

void loop() {
    long startA = angles[0];
    motor.moveTo(startA);
    motor.run();
    for(int i = 1; i < cp.getRowsCount(); i++) {
        int steps = angles[i]/0.225;
        motor.setMaxSpeed(steps/timestep[i]);
        motor.runToNewPosition(steps);
  }
}

//   for(int row = 0; row < cp.getRowsCount(); row++) {
//     Serial.print("row = ");
//     Serial.print(row, DEC);
//     Serial.print(", column_1 = ");
//     Serial.print(column_1[row], DEC);
//     Serial.print(", column_2 = ");
//     Serial.println(column_2[row], DEC);
//   }
// output parsed values (allows to check that the file was parsed correctly)
//  cp.print(); // assumes that "Serial.begin()" was called before (otherwise it won't work)