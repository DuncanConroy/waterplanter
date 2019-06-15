#ifndef WaterSensor_h
#define WaterSensor_h

#include <Arduino.h>
#include <timer.h>
#include "logger.h"
#include "led.h"

class WaterSensor
{
  public:
    WaterSensor(const Logger& logger,
                const LED& led,
                int sensorPin,
                int durationCalibration,
                int blinkDurationCalibration,
                float measuringPercentageThreshold
                ): calibrationTimer(this, &WaterSensor::internalCalibrate),
                _led(led) {
                  _logger = logger;
//                  _led = led;
                  pinSensor = sensorPin;
                  calibrationDuration = durationCalibration;
                  blinkCalibrationDuration = blinkDurationCalibration;
                  measuringThresholdPercentage = measuringPercentageThreshold;
                  
                  calibrationTimer.setInterval(blinkCalibrationDuration);
                  calibrationTimer.start();
                }
    bool isCalibrated();
    bool needsWatering();
    void calibrate();
    void reset();
    void init();
  private:
    bool checkCalibrationSuccessful();
    int doRead();
    void invalidateSensors();
    void internalCalibrate();
    void stopCalibration();
    int calculateThreshold();
    Logger& _logger;
    const LED& _led;
    TimerForMethods<WaterSensor> calibrationTimer;
    int pinSensor;
    int blinkCalibrationDuration;
    int calibrationDuration;
    int calibrationTimeLeft;
    int measuringThreshold;
    float measuringThresholdPercentage;
    int historyWetValue;
    int historyDryValue;
    bool calibrating;
};

#endif
