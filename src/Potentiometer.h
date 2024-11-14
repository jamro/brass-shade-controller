#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

class Potentiometer {
public:
    Potentiometer(int pin);                       // Constructor
    void setup();                                 // Initializes the potentiometer
    int readValue();                              // Reads a new value
    int getValue();                               // Gets the running average

private:
    int _pin;                                     // Analog pin connected to potentiometer
    int _sampleSize;                              // Number of samples for running average
    int *_samples;                                // Array to store recent samples
    int _sampleIndex;                             // Index for current sample
    int _sampleCount;                             // Current number of samples
    int _total;                                   // Sum of samples for running average

    void addSample(int value);                    // Adds a new sample to the running average
};

#endif