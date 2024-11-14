#ifndef COVER_H
#define COVER_H

#include <Arduino.h>
#include "Potentiometer.h"
#include "HomeAssistant.h"

class Cover {
public:
  Cover(String haApiUrl, String entityId, String batteryId, int pin);    // Constructor
  void setup();                                                       // Initializes the cover
  void update();                                                      // Update logic
  int getPosition();
  int getBattery();

private:
  Potentiometer _potentiometer;                                     
  int _cooldown;
  int _batteryTimer;
  int _sentPos;
  int _prevPos;
  String _haApiUrl;
  String _entityId;
  String _batteryId;
  int _battery;
};

#endif