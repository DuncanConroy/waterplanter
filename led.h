#ifndef LED_h
#define LED_h

#include <Arduino.h>
#include "logger.h"

class LED
{
  public:
    LED(const Logger& logger,
        int LEDPin);
    void toggle(bool on);
    bool isOn();
  private:
    Logger&_logger;
    int pinLED;
    bool _on;
};

#endif
