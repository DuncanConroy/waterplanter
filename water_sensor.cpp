#include <Arduino.h>
#include <timer.h>
#include "water_sensor.h"
#include "logger.h"
#include "led.h"

#define SENSOR_MIN 0
#define SENSOR_MAX 1023

//WaterSensor::WaterSensor(const Logger& logger,
//                         const LED& led,
//                         int sensorPin,
//                         int durationCalibration,
//                         int blinkDurationCalibration,
//                         float measuringPercentageThreshold)
//{
//  _logger = logger;
//  _led = led;
//  pinSensor = sensorPin;
//  calibrationDuration = durationCalibration;
//  blinkCalibrationDuration = blinkDurationCalibration;
//  measuringThresholdPercentage = measuringPercentageThreshold;
//}

void WaterSensor::init() {
  pinMode(pinSensor, INPUT);
  reset();
}

void WaterSensor::invalidateSensors()
{
  historyDryValue = -1;
  historyWetValue = SENSOR_MAX;
  measuringThreshold = -1;
}

void WaterSensor::calibrate() {
  if (calibrating) return;
  invalidateSensors();
  calibrationTimeLeft = calibrationDuration;
  calibrating = true;

//  calibrationTimer(this, this::internalCalibrate);
//  calibrationTimer.setCallback(WaterSensor::internalCalibrate);
//  calibrationTimer = new TimerForMethods<WaterSensor>(this, &WaterSensor::internalCalibrate);
//  calibrationTimer(*this, &WaterSensor::internalCalibrate);
  
}

void WaterSensor::internalCalibrate() {
  _logger.log("internalCalibrate");
  
  calibrationTimeLeft -= blinkCalibrationDuration;
  if(calibrationTimeLeft <=0) {
    stopCalibration();
    return;
  }

  _led.toggle(!_led.isOn());
  _logger.log("calibrationTimeLeft:" + String(calibrationTimeLeft));
  _logger.log("ledOn:" + String(_led.isOn()));

  updateValues();
}

int WaterSensor::updateValues() {
  int currentValue = doRead();
  _logger.log("current: " + String(currentValue)
    + ", threshold: " + String(measuringThreshold)
    + ", historyDry: " + String(historyDryValue)
    + ", historyWet: " + String(historyWetValue));
  
  historyDryValue = max(historyDryValue, currentValue);
  historyWetValue = min(historyWetValue, currentValue);
  
  measuringThreshold = calculateThreshold();

  return currentValue;
}

void WaterSensor::stopCalibration() {
  calibrationTimer.pause();
  calibrationTimer.reset();
  calibrating = false;
  _led.toggle(false);
  _logger.log("calibration finished. Stopping Timer.");

  if (!checkCalibrationSuccessful()) {
    _logger.log("Calibration unsuccessful. Going back to calibration mode.");
    reset();
  }
}

int WaterSensor::calculateThreshold()
{
  int diff = max(1, abs(historyDryValue - historyWetValue));
  _logger.log("hD: "+ String(historyDryValue) + " - diff: " + String(diff) + " * mTP: " + String(measuringThresholdPercentage));
  _logger.log("hD: "+ String(historyDryValue) + " - " + String((diff * measuringThresholdPercentage)));
  int result = constrain(
      historyDryValue - ( diff * measuringThresholdPercentage),
      min(historyWetValue + (SENSOR_MAX * .01), 0),
      SENSOR_MAX 
     );
         
  _logger.log("threshold: " + String(result));
  return result;
}

bool WaterSensor::isCalibrated()
{
  return !calibrating && historyDryValue != -1 && historyWetValue != SENSOR_MAX && measuringThreshold != -1;
}

bool WaterSensor::checkCalibrationSuccessful() {
  int diff = abs(historyDryValue - historyWetValue);
  if (diff < 100) {
    invalidateSensors();
    
    return false;
  }

  return true;
}

bool WaterSensor::needsWatering()
{  
  int analogWaterSensorValue = updateValues();
  
  // high sensor value means little conductivity -> dry soil
  return analogWaterSensorValue >= measuringThreshold;
}

int WaterSensor::doRead()
{;
  int i = 0;
  unsigned long int readValues = 0l;
  int iterations = 30; //should be max 32, because of SHORT overflow
  int currentValue = 0;
  while(i++ < iterations){
    currentValue = analogRead(pinSensor);
    readValues += currentValue;
//    char buffer [20];
//    sprintf(buffer, "current: %d, readValues: %d", currentValue, (int) readValues);
//    _logger.log(buffer);
  }
  int sensorValue = readValues / iterations;
  _logger.log(String(readValues) +"/"+ String(iterations) +"="+ sensorValue);
  
  return sensorValue;
}

void WaterSensor::reset()
{
  _logger.log("RESET");
  
  invalidateSensors();
  calibrate();
}
