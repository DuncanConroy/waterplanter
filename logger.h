#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

class Logger {
  public:
    Logger(bool productionMode);
    void println(const char[]);
    void println(const String &value);
    void println(int* value);
  private:
    bool _productionMode;
};

#endif
