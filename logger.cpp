#include <Arduino.h>
#include "logger.h"

Logger::Logger(bool productionMode) {
  _productionMode = productionMode;
  if (!Serial) return;
  if (!_productionMode) {
      Serial.begin(9600);
  }
}

void Logger::log(String value) {
    if(_productionMode || !Serial) return;

    Serial.println(value);
}
