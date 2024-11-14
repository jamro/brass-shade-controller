#include <WiFi.h>
#include "Cover.h"
#include "Screen.h"
#include "Config.h"
#include <Wire.h>

/* ---------------------------------------------- WIRING ----------------------------------------------

                              - XXX             XXX -
                              - XXX             SCL - OLED SCK
        potentiometer mid-pin -  VN             XXX -
        potentiometer mid-pin -  34             XXX -
        potentiometer mid-pin -  35             SDA - OLED SDA
        potentiometer mid-pin -  32             XXX -
        potentiometer mid-pin -  33             XXX -
                              -  25             XXX -
                              - XXX             XXX -
                              - XXX             XXX -
                              - XXX             XXX -
                              - XXX             XXX -
                              - XXX             XXX -
                              - XXX     ___     GND - 2x potentionmeter top pin / OLED GND
                              - XXX    |   |    3D3 - 2x potentionmeter bottom pin / OLED VCC

*/

Config cfg;
String API_TOKEN = "";
const int LED_PIN = 2;

const int SLIDER_PINS[] = {33, 32, 39, 34, 35};

Cover* covers[5];
Screen display;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // load config
  cfg.setup();
  API_TOKEN = cfg.getHomeAssistantToken();

  // config OLED display
  display.setup();
  display.displayText("Connecting...");
  delay(500);

  // connect to WIFI
  if(!connectWifi(cfg.getWifiSSID(), cfg.getWifiPassword())) {
    display.displayText("Error: Cannot connect");
    while(true) {
      cfg.readCommand();
      delay(50);
    }
  }

  // create covers
  for (int i = 0; i < 5; i++) {
    covers[i] = new Cover(cfg.getHomeAssistantUrl(), cfg.getCoverEntityId(i), cfg.getBatteryEntityId(i), SLIDER_PINS[i]);
    covers[i]->setup();
  }

  display.displayText("Ready!");
  delay(500);
}

void loop() {
  cfg.readCommand();
  for (int i = 0; i < 5; i++) {
    covers[i]->update();
  }

  int batteryLevels[5] = {
    covers[0]->getBattery(),
    covers[1]->getBattery(),
    covers[2]->getBattery(),
    covers[3]->getBattery(),
    covers[4]->getBattery(),
  };
  int positions[5] = {
    covers[0]->getPosition(),
    covers[1]->getPosition(),
    covers[2]->getPosition(),
    covers[3]->getPosition(),
    covers[4]->getPosition(),
  };
  display.drawCovers(positions, batteryLevels);
}

