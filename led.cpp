#include <Arduino.h>
#include "logger.h"
#include "led.h"

LED::LED(const Logger& logger,
         int LEDPin) {
  _on = false;
  _logger = logger;
  pinLED = LEDPin;

  pinMode(pinLED, OUTPUT);
  toggle(false);
}

void LED::toggle(bool on)
{
  _logger.log("toggle("+String(on)+")");
  _on = on;
  digitalWrite(pinLED, _on);
  _logger.log("LED: " + String(_on));
}

bool LED::isOn()
{
  return _on;
}
