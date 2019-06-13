#ifndef WaterPump_h
#define WaterPump_h

#include <Arduino.h>
#include "logger.h"

class WaterPump
{
  public:
    WaterPump(Logger logger,
              int LEDPin,
              int MOSFETPin,
              int durationActivation);
    void activate();
  private:
    Logger _logger;
    int pinLED;
    int pinMOSFET;
    int activationDuration;
    void switchPump(boolean state);
};

#endif
