#include <Arduino.h>
#include "logger.h"

Logger::Logger(bool productionMode) {
    if (!Serial) return;

    _productionMode = productionMode;

    if (_productionMode) {
        Serial.begin(9600);
    }
}

Logger::println(const char value[]) {
    if(!_productionMode || !Serial) return;

    Serial.println(value);
}

Logger::println(const String &value) {
    if(!_productionMode || !Serial) return;

    Serial.println(value);
}

Logger::println(int* value) {
    if(!_productionMode || !Serial) return;

    Serial.println(value);
}
