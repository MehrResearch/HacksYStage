#ifdef DRIVER_L298N

#include <Arduino.h>
#include <Stepper.h>

// x and y stepper motors
Stepper stepper_x(1000, 8, 9, 10, 11);
Stepper stepper_y(1000, 4, 5, 6, 7);

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  // turn stepper motor
  stepper_x.setSpeed(60);
  stepper_y.setSpeed(60);
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < 60; i++)
  {
    stepper_x.step(1);
    delay(50);
    stepper_y.step(1);
    delay(50);
  }
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
  // reverse direction
  for (int i = 0; i < 60; i++)
  {
    stepper_x.step(-1);
    delay(50);
    stepper_y.step(-1);
    delay(50);
  }
  // wait for a second
  delay(1000);
}

#endif