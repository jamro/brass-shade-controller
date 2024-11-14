#ifndef HOMEASSISTANT_H
#define HOMEASSISTANT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

extern String API_TOKEN;

bool connectWifi(String ssid, String pass);
bool openWindowCover(String haApiUrl, String entityId, int pos);
int getCoverBattery(String haApiUrl, String entityId);

#endif