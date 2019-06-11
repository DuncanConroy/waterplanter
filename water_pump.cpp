#include <Arduino.h>
#include "water_pump.h"

WaterPump::WaterPump(int LEDPin,
                     int MOSFETPin,
                     int durationActivation) {
  pinLED = LEDPin;
  pinMOSFET = MOSFETPin;
  activationDuration = durationActivation;

  pinMode(pinMOSFET, OUTPUT);
  pinMode(pinLED, OUTPUT);
  switch(0);
}

void WaterPump::activate()
{
  #ifndef PRODUCTION_MODE
    Serial.println("Activating pump for " + String(activationDuration / 1000) + " seconds.");
  #endif

  switchPump(1);
  delay(activationDuration);
  switchPump(0);
}

void WaterPump::switchPump(boolean state)
{
  digitalWrite(pinMOSFET, state);
  digitalWrite(pinLED, state);
  Serial.println("LED: " + String(state));
}
