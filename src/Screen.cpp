#include "Screen.h"

#define SCREEN_WIDTH 128  // Replace with your screen width
#define SCREEN_HEIGHT 32  // Replace with your screen height
#define OLED_RESET    -1  // Reset pin, -1 if using only 4 pins

Screen::Screen() : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {

}

void Screen::setup() {
  Wire.begin(21, 22); // SDA and SCL pins for ESP32
  _display.begin(SSD1306_PAGEADDR, 0x3C);
}

void Screen::displayText(char* text) {
  _display.clearDisplay();
  _display.setTextSize(1); 
  _display.setTextColor(SSD1306_WHITE); 
  _display.setCursor(0,0);
  _display.println(text);
  _display.display();
}


void Screen::drawCovers(int positions[5], int energies[5]) {
  int windowWidth = 15;                // Width of each window
  int windowHeight = 18;               // Height of each window, slightly reduced to fit
  int batteryHeight = 6;               // Height of each battery
  int batteryCapWidth = 2;             // Width of the battery cap
  int batterySpacing = 3;              // Space between window and battery
  int spacing = (SCREEN_WIDTH - (5 * windowWidth)) / 6; // Calculate spacing to distribute windows evenly

  // Clear the display for updating window levels
  _display.clearDisplay();

  for (int i = 0; i < 5; i++) {
    int windowX = spacing + i * (windowWidth + spacing); // X position for each window
    int windowY = SCREEN_HEIGHT - windowHeight - batteryHeight - batterySpacing; // Move window down slightly

    // Draw window outline (body)
    _display.drawRect(windowX, windowY, windowWidth, windowHeight, SSD1306_WHITE);

    // Calculate height of filled part based on energy percentage
    int fillHeight = map(max(0.0, positions[i]-25.0), 75, 0, 0, windowHeight - 2);

    // Draw window level inside the window body (from bottom up)
    if (fillHeight > 0) {
      _display.fillRect(windowX + 1, windowY + 1, windowWidth - 2, fillHeight, SSD1306_WHITE);
    }

    // Draw the battery below the window
    int batteryX = windowX;
    int batteryY = windowY + windowHeight + batterySpacing;

    // Calculate the fill width based on energy percentage
    int fillWidth = map(energies[i], 0, 100, 0, windowWidth - 4); // Adjust width to fit cap

    // Draw battery outline
    _display.drawRect(batteryX, batteryY, windowWidth, batteryHeight, SSD1306_WHITE);

    // Draw battery cap on the right side
    _display.fillRect(batteryX + windowWidth, batteryY + 2, batteryCapWidth, batteryHeight - 4, SSD1306_WHITE);

    // Fill battery level
    if (fillWidth > 0) {
      _display.fillRect(batteryX + 1, batteryY + 1, fillWidth, batteryHeight - 2, SSD1306_WHITE);
    }
  }

  // Update the display to show all windows and batteries
  _display.display();
}