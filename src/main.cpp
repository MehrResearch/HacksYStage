#ifdef DRIVER_DRV8825

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_DotStar.h>

#define DOTSTAR_NUMPIXELS 64

#define X_ENABLE 6
#define X_STEP 7
#define X_DIR 8

#define Y_ENABLE 9
#define Y_STEP 10
#define Y_DIR 12

// delay between enabling stepper drivers and taking steps
#define ENABLE_DELAY_MS 50
#define STEPPER_PULSE_US 1000

// truncate long input to avoid OOM
#define MAX_CMD_LEN 64

Adafruit_DotStar matrix(DOTSTAR_NUMPIXELS, DOTSTAR_BRG, &SPI);

String cmd = "";

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
  delay(ENABLE_DELAY_MS);
  for (int i = 0; i < n; i++)
  {
    digitalWrite(X_STEP, HIGH);
    delayMicroseconds(STEPPER_PULSE_US);
    digitalWrite(X_STEP, LOW);
    delayMicroseconds(STEPPER_PULSE_US);
  }
  delay(ENABLE_DELAY_MS);
  digitalWrite(X_ENABLE, HIGH);
}

/// @brief Take n steps in the y direction
/// @param dir Direction to step in
/// @param n Number of steps
void y_step(bool dir, int n)
{
  digitalWrite(Y_DIR, dir);
  digitalWrite(Y_ENABLE, LOW);
  delay(ENABLE_DELAY_MS);
  for (int i = 0; i < n; i++)
  {
    digitalWrite(Y_STEP, HIGH);
    delayMicroseconds(STEPPER_PULSE_US);
    digitalWrite(Y_STEP, LOW);
    delayMicroseconds(STEPPER_PULSE_US);
  }
  delay(ENABLE_DELAY_MS);
  digitalWrite(Y_ENABLE, HIGH);
}

/// @brief Turn on DotStar matrix backlight
void backlight_on(u8 r, u8 g, u8 b)
{
  u32 color = g;
  color <<= 8;
  color |= r;
  color <<= 8;
  color |= b;
  matrix.fill(color);
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

  Serial.begin(115200);
  Serial.setTimeout(100);
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
// backlight_on r g b: backlight_on(r, g, b) (0 <= r, g, b <= 255)
// backlight_off: backlight_off()
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
  else if (cmd.startsWith("backlight_on"))
  {
    String rgb = cmd.substring(cmd.indexOf(' ') + 1);
    u8 r = rgb.substring(0, rgb.indexOf(' ')).toInt();
    u8 g = rgb.substring(rgb.indexOf(' ') + 1, rgb.lastIndexOf(' ')).toInt();
    u8 b = rgb.substring(rgb.lastIndexOf(' ') + 1).toInt();
    #ifdef DEBUG
      Serial.print("r: ");
      Serial.print(r);
      Serial.print(" g: ");
      Serial.print(g);
      Serial.print(" b: ");
      Serial.println(b);
    #endif
    backlight_on(r, g, b);
  }
  else if (cmd == "backlight_off")
  {
    backlight_off();
  }
  else {
    Serial.print("Unknown command: ");
    Serial.println(cmd);
  }
}

void loop()
{
  // Read and echo each character until \r
  while (Serial.available())
  {
    char c = Serial.read();

    //detect arrow keys
    if (c == 27)
    {
      char seq[2];
      Serial.readBytes(seq, 2);
      switch (seq[1])
      {
      case 'A':
        x_step(false, 16);
        break;
      case 'B':
        x_step(true, 16);
        break;
      case 'C':
        y_step(true, 16);
        break;
      case 'D':
        y_step(false, 16);
        break;
      }
      continue;
    }

    Serial.print(c);
    if (c == '\r' || cmd.length() > MAX_CMD_LEN)
    {
      Serial.println("");
      cmd.trim();
      handle_command(cmd);
      cmd = "";
    }
    else
    {
      cmd += c;
    }
  }
}

#endif