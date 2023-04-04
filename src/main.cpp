#ifdef DRIVER_DRV8825

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>

#define DOTSTAR_NUMPIXELS 64
// SCK, MOSI pins for Arduino Mega hardware SPI
#define SCK 52
#define MOSI 51

#define X_ENABLE 7
#define X_STEP 8
#define X_DIR 9

#define Y_ENABLE 10
#define Y_STEP 11
#define Y_DIR 12

// delay between enabling stepper drivers and taking steps
#define ENABLE_DELAY_US 500
#define STEPPER_PULSE_US 1000

bool dir = HIGH;
Adafruit_DotStar matrix(DOTSTAR_NUMPIXELS, MOSI, SCK, DOTSTAR_BRG);

// Commands

/// @brief Turn on onboard LED
void led_on()
{
  digitalWrite(LED_BUILTIN, HIGH);
}

/// @brief Turn off onboard LED
void led_off()
{
  digitalWrite(LED_BUILTIN, LOW);
}

/// @brief Take n steps in the x direction
/// @param dir Direction to step in
/// @param n Number of steps
void x_step(bool dir, int n)
{
  digitalWrite(X_DIR, dir);
  digitalWrite(X_ENABLE, LOW);
  delay(ENABLE_DELAY_US);
  for (int i = 0; i < n; i++)
  {
    digitalWrite(X_STEP, HIGH);
    delayMicroseconds(STEPPER_PULSE_US);
    digitalWrite(X_STEP, LOW);
    delayMicroseconds(STEPPER_PULSE_US);
  }
  digitalWrite(X_ENABLE, HIGH);
}

/// @brief Take n steps in the y direction
/// @param dir Direction to step in
/// @param n Number of steps
void y_step(bool dir, int n)
{
  digitalWrite(Y_DIR, dir);
  digitalWrite(Y_ENABLE, LOW);
  delay(ENABLE_DELAY_US);
  for (int i = 0; i < n; i++)
  {
    digitalWrite(Y_STEP, HIGH);
    delayMicroseconds(STEPPER_PULSE_US);
    digitalWrite(Y_STEP, LOW);
    delayMicroseconds(STEPPER_PULSE_US);
  }
  digitalWrite(Y_ENABLE, HIGH);
}

/// @brief Turn on DotStar matrix backlight
void backlight_on()
{
  matrix.fill(0x00050505);
  matrix.show();
}

/// @brief Turn off DotStar matrix backlight
void backlight_off()
{
  matrix.fill(0x00000000);
  matrix.show();
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(X_ENABLE, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  digitalWrite(X_ENABLE, HIGH);

  pinMode(Y_ENABLE, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  digitalWrite(Y_ENABLE, HIGH);

  matrix.begin();
  matrix.show();

  Serial.begin(9600);
  Serial.setTimeout(10000);
  Serial.println("Serial ready");
}

void debug_loop()
{
  led_on();
  x_step(true, 2000);
  x_step(false, 2000);
  led_off();
  y_step(true, 2000);
  y_step(false, 2000);
}

void handle_command(String cmd)
{
// commands:
// x_step +/-n: x_step(true/false, n)
// y_step +/-n: y_step(true/false, n)
// led_on: led_on()
// led_off: led_off()
// backlight_on: backlight_on()
// backlight_off: backlight_off()
#ifdef DEBUG
  Serial.print("Received: ");
  Serial.println(cmd);
#endif

  if (cmd.startsWith("x_step"))
  {
    // x_step +/-n
    int n = cmd.substring(7).toInt();
    #ifdef DEBUG
      Serial.print("axis: x; n: ");
      Serial.println(n);
    #endif
    x_step(n > 0, abs(n));
  }
  else if (cmd.startsWith("y_step"))
  {
    // y_step +/-n
    int n = cmd.substring(7).toInt();
    #ifdef DEBUG
      Serial.print("axis: y; n: ");
      Serial.println(n);
    #endif  
    y_step(n > 0, abs(n));
  }
  else if (cmd == "led_on")
  {
    led_on();
  }
  else if (cmd == "led_off")
  {
    led_off();
  }
  else if (cmd == "backlight_on")
  {
    backlight_on();
  }
  else if (cmd == "backlight_off")
  {
    backlight_off();
  }
}

void loop()
{
  // Read and execute a command
  if (Serial.available() > 0)
  {
    String cmd = Serial.readStringUntil('\r');
    handle_command(cmd);
  }
}

#endif