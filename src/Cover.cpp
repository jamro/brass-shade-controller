#include "Cover.h"

const int LED_PIN = 2;
const int COOLDOWN_MAX = 10;

Cover::Cover(String haApiUrl, String entityId, String batteryId, int pin) : _potentiometer(pin) {
  _entityId = entityId;
  _haApiUrl = haApiUrl;
  _sentPos = 0;
  _prevPos = 0;
  _cooldown = COOLDOWN_MAX;
  _batteryTimer = random(0, 101);
  _batteryId = batteryId;
  _battery = 100;
}

void Cover::setup() {
  _potentiometer.setup();
  _potentiometer.readValue();
  delay(250);
  _potentiometer.readValue();

  _sentPos = _potentiometer.getValue();
  _prevPos = _sentPos;
}

int Cover::getBattery() {
  return _battery;
}

int Cover::getPosition() {
  return _potentiometer.getValue();
}

void Cover::update() {
  _potentiometer.readValue();
  int value = _potentiometer.getValue();
  int dv = _prevPos - value;
  _prevPos = value;

  if(dv != 0) {
    _cooldown = COOLDOWN_MAX;
  }

  if(_cooldown == 0 && abs(_sentPos - value) > 5) {    
    _sentPos = value;
    _cooldown = COOLDOWN_MAX;
    digitalWrite(LED_PIN, HIGH);
    openWindowCover(_haApiUrl, _entityId, _sentPos);
    digitalWrite(LED_PIN, LOW);
  }

  if(_cooldown > 0) {
    _cooldown--;
  }

  _batteryTimer--;
  if(_batteryTimer <= 0) {
    _batteryTimer = 12000; // 10min at cycle 50ms
    int val = getCoverBattery(_haApiUrl, _batteryId);
    if (val >= 0) {
      _battery = val;
    }
  }
}