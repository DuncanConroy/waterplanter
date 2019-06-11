#include <Arduino.h>
#include "water_sensor.h"

#define SENSOR_MIN 0
#define SENSOR_MAX 1000

WaterSensor::WaterSensor(int sensorPin,
                         int LEDPin,
                         int MOSFETPin,
                         int durationCalibration,
                         int blinkDurationCalibration,
                         float measuringPercentageThreshold)
{
  pinSensor = sensorPin;
  pinLED = LEDPin;
  pinMOSFET = MOSFETPin;
  calibrationDuration = durationCalibration;
  blinkCalibrationDuration = blinkDurationCalibration;
  measuringThresholdPercentage = measuringPercentageThreshold;
}

void WaterSensor::init() {
  pinMode(A0, INPUT);
  pinMode(pinMOSFET, OUTPUT);
  pinMode(pinLED, OUTPUT);
  reset();
}

void WaterSensor::invalidateSensors()
{
  dryValue = -1;
  wetValue = -1;
  measuringThreshold = -1;
}

void WaterSensor::calibrate() {
  invalidateSensors();
  boolean blinkOn = true;
  int timeToCalibrate = calibrationDuration;
  while(timeToCalibrate > 0) {
    #ifndef PRODUCTION_MODE
      Serial.println("timeToCalibrate: " + String(timeToCalibrate));
    #endif
    digitalWrite(pinLED, blinkOn);
    blinkOn = !blinkOn;
    timeToCalibrate -= blinkCalibrationDuration;
    int currentValue = doRead();
    #ifndef PRODUCTION_MODE
      Serial.println("current: " + String(currentValue) +
      ", dry: " + String(dryValue) 
      + ", wet: " + String(wetValue) 
      + ", threshold: " + String(measuringThreshold) +
      ", historyDry: " + String(historyDryValue) +
      ", historyWet: " + String(historyWetValue));
    #endif
    
    if (dryValue == -1 || dryValue > currentValue) {
      dryValue = currentValue;
      historyDryValue = max(historyDryValue, dryValue);
    }
    if (wetValue == -1 || wetValue < currentValue) {
      wetValue = currentValue;
      historyWetValue = min(historyWetValue, wetValue);
    }
    
    measuringThreshold = calculateThreshold();
    delay(blinkCalibrationDuration);
  }
  
  checkCalibrationSuccessful();
}

int WaterSensor::calculateThreshold()
{
  int diff = max(1, abs(historyDryValue - historyWetValue));
  Serial.println("hD: "+ String(historyDryValue) + " - diff: " + String(diff) + " * mTP: " + String(measuringThresholdPercentage));
  Serial.println("hD: "+ String(historyDryValue) + " - " + String((diff * measuringThresholdPercentage)));
  int result = constrain(
      historyDryValue - ( diff * measuringThresholdPercentage),
      historyDryValue - (SENSOR_MAX * .01), 
      min(historyWetValue + (SENSOR_MAX * .01), 1000)
    );

    
         
  Serial.println("threshold: " + String(result));
  return result;
}

boolean WaterSensor::isCalibrated()
{
    return dryValue != -1 && wetValue != -1 && measuringThreshold != -1;
}

void WaterSensor::checkCalibrationSuccessful() {
  if (dryValue == wetValue) {
    dryValue = -1;
    wetValue = -1;
    measuringThreshold = -1;
  }
}

boolean WaterSensor::needsWatering()
{
  if (measuringThreshold < 10) {
    calibrate();
    return;
  }
  
  int analogWaterSensorValue = doRead();
  #ifndef PRODUCTION_MODE
    Serial.println("analog:" + String(analogWaterSensorValue) + ", activationThreshold: " + String(measuringThreshold));
  #endif
  
  // high sensor value means little conductivity -> dry soil
  return analogWaterSensorValue >= measuringThreshold;
}

int WaterSensor::doRead()
{
  digitalWrite(pinMOSFET, 1);
  delay(500);
  int i = 0;
  unsigned long int readValues = 0l;
  int iterations = 30; //should be max 32, because of a short overflow
  int currentValue = 0;
  while(i++ < iterations){
    currentValue = analogRead(pinSensor);
    readValues += currentValue;
    char buffer [20];
    sprintf(buffer, "current: %d, readValues: %d", currentValue, readValues);
    Serial.println(buffer);
  }
  int sensorValue = readValues / iterations;
  Serial.println(String(readValues) +"/"+ String(iterations) +"="+ sensorValue);
  Serial.println(":>>>" + String(sensorValue));
  delay(500);
  digitalWrite(pinMOSFET, 0);
  return sensorValue;
}

void WaterSensor::reset()
{
  Serial.println("RESET");
  
  historyDryValue = -1;
  historyWetValue = 1000;
  
  invalidateSensors();
  calibrate();
}