#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Screen {
public:
  Screen();   
  void setup();                         
  void displayText(char* text);  
  void drawCovers(int positions[5], int energies[5]);

private:
  Adafruit_SSD1306 _display;
};

#endif