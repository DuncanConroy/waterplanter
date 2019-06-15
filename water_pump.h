#ifndef WaterPump_h
#define WaterPump_h

#include <Arduino.h>
#include "logger.h"
#include "led.h"

class WaterPump
{
  public:
    WaterPump(const Logger& logger,
              const LED& led,
              int MOSFETPin,
              int durationActivation);
    void activate();
  private:
    Logger& _logger;
    const LED& _led;
    int pinMOSFET;
    int activationDuration;
    void switchPump(bool state);
};

#endif
