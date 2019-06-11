#ifndef WaterPump_h
#define WaterPump_h

#include <Arduino.h>

class WaterPump
{
  public:
    WaterPump(int LEDPin,
              int MOSFETPin,
              int durationActivation);
    void activate();
  private:
    int pinLED;
    int pinMOSFET;
    int activationDuration;
    void switchPump(boolean state);
};

#endif
