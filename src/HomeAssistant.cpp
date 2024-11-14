#include "HomeAssistant.h"

const int LED_PIN = 2;

bool connectWifi(String ssid, String pass) {
  const unsigned long timeout = 5000;  // 5 seconds timeout
  unsigned long startAttemptTime = millis();  // Record the start time

  digitalWrite(LED_PIN, HIGH);
  Serial.print("Connecting to Wi-Fi network: ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), pass.c_str());  // Start Wi-Fi connection

  // Loop until connected or timeout
  while (WiFi.status() != WL_CONNECTED) {
    // Check if the timeout has been reached
    if (millis() - startAttemptTime >= timeout) {
      Serial.println("\nError: Failed to connect to Wi-Fi: Timeout reached.");
      digitalWrite(LED_PIN, HIGH);
      return false;  // Return false to indicate connection failure
    }

    // Blink LED to indicate attempting to connect
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }

  // Connected successfully
  digitalWrite(LED_PIN, LOW);
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return true;
}

bool openWindowCover(String haApiUrl, String entityId, int pos) {
  Serial.print("Set cover ");
  Serial.print(entityId);
  Serial.print(" positino to ");
  Serial.println(pos);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // API URL endpoint
    String url = String(haApiUrl) + "/services/cover/set_cover_position";
    http.begin(url);

    // Set headers for Home Assistant API
    http.addHeader("Authorization", String("Bearer ") + API_TOKEN);
    http.addHeader("Content-Type", "application/json");

    // JSON payload
    String payload = "{\"entity_id\": \"" + String(entityId) + "\", \"position\": " + String(pos) + " }";

    // Make the POST request
    int httpResponseCode = http.POST(payload);

    // Check the response code
    if (httpResponseCode == 200) {
      http.end();
      return true;
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      http.end();
      return false;
    }
  } else {
    Serial.println("Error: Wi-Fi not connected (openWindowCover)");
    return false;
  }
}

int getCoverBattery(String haApiUrl, String entityId) {
  Serial.print("Getting battery level for ");
  Serial.println(entityId);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // API URL endpoint
    String url = String(haApiUrl) + "/states/" + String(entityId);
    http.begin(url);

    // Set headers for Home Assistant API
    http.addHeader("Authorization", String("Bearer ") + API_TOKEN);
    http.addHeader("Content-Type", "application/json");

    // Make the POST request
    int httpResponseCode = http.GET();

    // Check the response code
    if (httpResponseCode == 200) {

      String response = http.getString();
      // Check if the response contains "state"
      int startIndex = response.indexOf("\"state\":");
      if (startIndex != -1) {
        startIndex += 9;  // Move past `"state":` to the value
        int endIndex = response.indexOf("\"", startIndex);

        // Check if endIndex is valid
        if (endIndex != -1) {
          String batteryLevelStr = response.substring(startIndex, endIndex);

          // Convert to integer and verify conversion
          int batteryLevel = batteryLevelStr.toInt();
          if (batteryLevelStr.length() > 0 && batteryLevel >= 0 && batteryLevel <= 100) {
              http.end();
              return batteryLevel;  // Return valid battery level
          } else {
              Serial.println("Error: Invalid battery level format.");
          }
        } else {
          Serial.println("Error: End index for battery level not found.");
        }
      } else {
        Serial.println("Error: 'state' key not found in the response.");
      }

      http.end();
      return -1;  // Return -1 if there was an error

    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      http.end();
      return -1;
    }
  } else {
    Serial.println("Error: Wi-Fi not connected (getCoverBattery)");
    return -1;
  }
}