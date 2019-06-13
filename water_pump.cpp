#include <Arduino.h>
#include "water_pump.h"

WaterPump::WaterPump(Logger logger,
                     int LEDPin,
                     int MOSFETPin,
                     int durationActivation) {
  _logger = logger;
  pinLED = LEDPin;
  pinMOSFET = MOSFETPin;
  activationDuration = durationActivation;

  pinMode(pinMOSFET, OUTPUT);
  pinMode(pinLED, OUTPUT);
  switch(0);
}

void WaterPump::activate()
{
  _logger.println("Activating pump for " + String(activationDuration / 1000) + " seconds.");

  switchPump(1);
  delay(activationDuration);
  switchPump(0);
}

void WaterPump::switchPump(boolean state)
{
  digitalWrite(pinMOSFET, state);
  digitalWrite(pinLED, state);
  _logger.println("LED: " + String(state));
}
