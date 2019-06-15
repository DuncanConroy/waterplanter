#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

class Logger {
  public:
    Logger(bool productionMode);
    void log(String value);
  private:
    bool _productionMode;
};

#endif
