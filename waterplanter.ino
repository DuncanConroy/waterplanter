#include <Arduino.h>
#include "water_sensor.h"
#include "water_pump.h"

#define PUMP_MOSFET_PIN 3
#define PUMP_LED_PIN 4
#define WATER_SENSOR_MOSFET_PIN 5
#define WATER_SENSOR_LED_PIN 6
#define WATER_SENSOR_PIN A0
#define WATER_SENSOR_RESET_AFTER_PUMP_CYCLES 10

#define WATER_ACTIVATION_DURATION 10000
#define BLINK_DURATION_CALIBRATION 100
#define CALIBRATION_DURATION 500
#define MEASURING_THRESHOLD_PERCENTAGE .8
//#define PRODUCTION_MODE true

WaterSensor waterSensor(WATER_SENSOR_PIN,
                        WATER_SENSOR_LED_PIN,
                        WATER_SENSOR_MOSFET_PIN,
                        CALIBRATION_DURATION,
                        BLINK_DURATION_CALIBRATION,
                        MEASURING_THRESHOLD_PERCENTAGE);
WaterPump pump(PUMP_LED_PIN,
               PUMP_MOSFET_PIN,
               WATER_ACTIVATION_DURATION);

int waterSensorReset = 0;

void setup() {
  #ifndef PRODUCTION_MODE
    Serial.begin(9600);
  #endif
  waterSensor.init();
}

void loop() {
  if (!waterSensor.isCalibrated()) {
    waterSensor.calibrate();
  }

  if(++waterSensorReset >= WATER_SENSOR_RESET_AFTER_PUMP_CYCLES) {
    waterSensor.reset();
    waterSensorReset = 0;
  }

  delay(5000);
  
  if (waterSensor.needsWatering()) {
    pump.activate();
  }
}
