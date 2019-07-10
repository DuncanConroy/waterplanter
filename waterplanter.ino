#include <timer.h>
#include <timerManager.h>

#include <Arduino.h>
#include "logger.h"
#include "water_sensor.h"
#include "water_pump.h"

#define PUMP_MOSFET_PIN 3
#define LED_PIN 4
#define WATER_SENSOR_PIN A0

#define WATER_ACTIVATION_DURATION 10000
#define BLINK_DURATION_CALIBRATION 100
#define CALIBRATION_DURATION 30000
#define MEASURING_THRESHOLD_PERCENTAGE .9
#define MEASURING_INTERVAL 1000//5 * 60 * 1000;
//#define PRODUCTION_MODE true

WaterPump* pump;
WaterSensor* waterSensor;
LED* led;

void setup() {
  #ifdef PRODUCTION_MODE
  Logger logger(true);
  #else
  Logger logger(false);
  #endif

  led = new LED(logger, LED_PIN);
  
  pump = new WaterPump(logger,
                       *led,
                       PUMP_MOSFET_PIN,
                       WATER_ACTIVATION_DURATION);
  waterSensor = new WaterSensor(logger,
                                *led,
                                WATER_SENSOR_PIN,
                                CALIBRATION_DURATION,
                                BLINK_DURATION_CALIBRATION,
                                MEASURING_THRESHOLD_PERCENTAGE);
  waterSensor->init();
  
  //Start all the timers
  TimerManager::instance().start();
}

void loop() {
  //Update all the timers at once
  TimerManager::instance().update();
  
  if (!waterSensor->isCalibrated()) {
    waterSensor->calibrate();
    return;
  }

  delay(MEASURING_INTERVAL);
  
  if (waterSensor->needsWatering()) {
    pump->activate();
  }
}
