#include <Arduino.h>
#include "water_pump.h"
#include "logger.h"
#include "led.h"

WaterPump::WaterPump(const Logger& logger,
                     const LED& led,
                     int MOSFETPin,
                     int durationActivation
                     ): _led(led) {
  _logger = logger;
//  _led = led;
  pinMOSFET = MOSFETPin;
  activationDuration = durationActivation;

  pinMode(pinMOSFET, OUTPUT);
  switchPump(0);
}

void WaterPump::activate()
{
  _logger.log("Activating pump for " + String(activationDuration / 1000) + " seconds.");

  switchPump(1);
  delay(activationDuration);
  switchPump(0);
}

void WaterPump::switchPump(bool state)
{
  digitalWrite(pinMOSFET, state);
  _led.toggle(state);
}
