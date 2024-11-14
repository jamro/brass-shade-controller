#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin) {
  _pin = pin;
  _sampleSize = 4;
  _samples = new int[_sampleSize];
  _sampleIndex = 0;
  _sampleCount = 0;
  _total = 0;
}

void Potentiometer::setup() {
  pinMode(_pin, INPUT);
}

int Potentiometer::readValue() {
  int value = analogRead(_pin);
  addSample(value); // Add the new sample
  return value;
}

void Potentiometer::addSample(int value) {
  if (_sampleCount < _sampleSize) {
    _sampleCount++;
  } else {
    _total -= _samples[_sampleIndex]; // Remove the oldest value from the sum
  }

  _samples[_sampleIndex] = value;     // Store new value
  _total += value;                    // Add new value to sum
  _sampleIndex = (_sampleIndex + 1) % _sampleSize; // Update index
}

int Potentiometer::getValue() {
  if (_sampleCount == 0) return 0;    // Avoid division by zero
  float avgVal = (float)_total / _sampleCount;
  float perc = round(100.0*(avgVal / 4095.0));
  perc = max(float(0.0), perc);
  perc = min(float(100.0), perc);
  return round(perc);
}