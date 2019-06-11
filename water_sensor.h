#ifndef WaterSensor_h
#define WaterSensor_h

#include <Arduino.h>

class WaterSensor
{
  public:
    WaterSensor(int sensorPin,
                int LEDPin,
                int MOSFETPin,
                int durationCalibration,
                int blinkDurationCalibration,
                float measuringPercentageThreshold);
    boolean isCalibrated();
    boolean needsWatering();
    void calibrate();
    void reset();
    void init();
  private:
    void checkCalibrationSuccessful();
    int doRead();
    void invalidateSensors();
    int calculateThreshold();
    int pinLED;
    int pinSensor;
    int pinMOSFET;
    int blinkCalibrationDuration;
    int calibrationDuration;
    int dryValue;
    int wetValue;
    int measuringThreshold;
    float measuringThresholdPercentage;
    int historyWetValue;
    int historyDryValue;
};

#endif
