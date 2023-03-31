#ifdef DRIVER_DRV8825

#include <Arduino.h>

#define X_ENABLE 7
#define X_STEP 8
#define X_DIR 9

#define Y_ENABLE 10
#define Y_STEP 11
#define Y_DIR 12

bool dir = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(X_ENABLE, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  pinMode(X_DIR, OUTPUT);

  pinMode(Y_ENABLE, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, dir);
  digitalWrite(X_ENABLE, HIGH);
  digitalWrite(Y_ENABLE, HIGH);
  digitalWrite(X_DIR, dir);
  digitalWrite(Y_DIR, dir);
  for (int i = 0; i < 2000; i++) {
    digitalWrite(X_STEP, HIGH);
    digitalWrite(Y_STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(X_STEP, LOW);
    digitalWrite(Y_STEP, LOW);
    delayMicroseconds(1000);
  }

  digitalWrite(X_ENABLE, LOW);
  digitalWrite(Y_ENABLE, LOW);
  delay(1000);

  // flip direction
  dir = !dir;
}

#endif