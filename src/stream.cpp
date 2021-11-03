#include <Arduino.h>
#include <AccelStepper.h>

long timestep;
long angle;

AccelStepper motor(AccelStepper::DRIVER,9,8);

void setup() {
    Serial.begin(9600);
    motor.setAcceleration(1000);
}

void loop() {
    timestep = Serial.parseInt();
    angle = Serial.parseInt();
    int steps = angle/0.225;
    motor.setMaxSpeed(steps/timestep);
    motor.runToNewPosition(steps);
}