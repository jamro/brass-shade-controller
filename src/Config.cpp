#include "Config.h"

int parseCommand(String command, String tokenArray[], int maxTokens) {
  int count = 0;  // Token counter
  int start = 0;  // Start of each token

  while (start < command.length() && count < maxTokens) {
    while (start < command.length() && command[start] == ' ') {
      start++;
    }
    int end = command.indexOf(' ', start);
    if (end == -1) {
      end = command.length();
    }
    if (start < end) {
      tokenArray[count++] = command.substring(start, end);
    }
    start = end + 1;
  }
  return count;
}

Config::Config() {

}

void Config::setup() {
  _preferences.begin("config", false);
  _wifiSSID = _preferences.getString("wifiSSID", "my_wifi");
  _wifiPassword = _preferences.getString("wifiPassword", "secret");
  _homeAssistantUrl = _preferences.getString("haUrl", "http://default-url");
  _homeAssistantToken = _preferences.getString("haToken", "defaultToken");

  for (int i = 0; i < 5; i++) {
    String key = "coverEntityId" + String(i + 1);
    _coverEntityId[i] = _preferences.getString(key.c_str(), ""); 
  }
  for (int i = 0; i < 5; i++) {
    String key = "batEntityId" + String(i + 1); 
    _batteryEntityId[i] = _preferences.getString(key.c_str(), ""); 
  }

  Serial.println("Configuration loaded successfully.");
}

String Config::getWifiSSID() {
  return _wifiSSID;
}

String Config::getWifiPassword() {
  return _wifiPassword;
}

String Config::getHomeAssistantUrl() {
  return _homeAssistantUrl;
}

String Config::getHomeAssistantToken() {
  return _homeAssistantToken;
}

String Config::getCoverEntityId(int index) {
  return _coverEntityId[index];
}

String Config::getBatteryEntityId(int index) {
  return _batteryEntityId[index];
}

void Config::_saveConfig() {
  _preferences.putString("wifiSSID", _wifiSSID);
  _preferences.putString("wifiPassword", _wifiPassword);
  _preferences.putString("haUrl", _homeAssistantUrl);
  _preferences.putString("haToken", _homeAssistantToken);

  for (int i = 0; i < 5; i++) {
    String key = "coverEntityId" + String(i + 1);  // Create unique key for each cover entity ID
    _preferences.putString(key.c_str(), _coverEntityId[i]);
  }

  for (int i = 0; i < 5; i++) {
    String key = "batEntityId" + String(i + 1);  // Create unique key for each battery entity ID
    _preferences.putString(key.c_str(), _batteryEntityId[i]);
  }
  Serial.println("Configuration saved!");
}

void Config::readCommand() {
  if (Serial.available() == 0) {
    return;
  }
  String tokens[MAX_CMD_TOKENS];
  String command = Serial.readStringUntil('\n');
  command.trim();

  int tokenCount = parseCommand(command, tokens, MAX_CMD_TOKENS);

  Serial.println(String("$> " + command));

  if(tokenCount > 0 && tokens[0] == "help") {
    Config::_runHelp(tokens, tokenCount);
  } else if(tokenCount > 0 && tokens[0] == "config") {
    Config::_runConfig(tokens, tokenCount);
  } else {
    Serial.println("Error: unknown command " + String(tokens[0]));
  }
}

void Config::_runHelp(String tokens[MAX_CMD_TOKENS], int tokenCount) {
  Serial.println("Available Commands");
  Serial.println(" - help: shows this help message");
  Serial.println(" - config: configure device");
  Serial.println("");
}

void Config::_runConfig(String tokens[MAX_CMD_TOKENS], int tokenCount) {
  if(tokenCount < 2) {
    Serial.println("Read and write configuration properties");
    Serial.println("");
    Serial.println("Available Commands:");
    Serial.println(" - config list: show all properties");
    Serial.println(" - config set [prop] [value]: configure property");
    Serial.println("");
    return;
  }
  if(tokenCount >= 2 && tokens[1] == "list") {
    Serial.println("Configuration Properties:");
    Serial.println(" - wifiSSID = '" + String(_wifiSSID) + "'");
    Serial.println(" - wifiPassword = '" + String(_wifiPassword) + "'");
    Serial.println(" - homeAssistantUrl = '" + String(_homeAssistantUrl) + "'");
    Serial.println(" - homeAssistantToken = '" + String(_homeAssistantToken).substring(0, 10) + "...'");
    for (int i = 0; i < 5; i++) {
      Serial.println(" - coverEntityId" + String(i+1) + " = '" + String(_coverEntityId[i]) + "'");
    }
    for (int i = 0; i < 5; i++) {
      Serial.println(" - batteryEntityId" + String(i+1) + " = '" + String(_batteryEntityId[i]) + "'");
    }
    return;
  }
  if(tokenCount < 4 && tokens[1] == "set") {
    Serial.println("Error: property name and value are required!");
    return;
  }
  if(tokenCount >=4 && tokens[1] == "set") {
    String prop = tokens[2];
    String val = tokens[3];

    Serial.println(String(prop) +  " = " + String(val));

    if(prop == "wifiSSID") {
      _wifiSSID = val;
    } else if(prop == "wifiPassword") {
      _wifiPassword = val;
    } else if(prop == "homeAssistantUrl") {
      _homeAssistantUrl = val;
    } else if(prop == "homeAssistantToken") {
      _homeAssistantToken = val;
    } else if (prop.startsWith("coverEntityId")) {
      int index = prop.substring(13).toInt() - 1;  // Extract index after "coverEntityId"
      if (index >= 0 && index < 5) {
        _coverEntityId[index] = val;
      } else {
        Serial.println("Error: Invalid index for coverEntityId. Must be 1-5.");
        return;
      }
    } else if (prop.startsWith("batteryEntityId")) {
      int index = prop.substring(15).toInt() - 1;  // Extract index after "batteryEntityId"
      if (index >= 0 && index < 5) {
        _batteryEntityId[index] = val;
      } else {
        Serial.println("Error: Invalid index for batteryEntityId. Must be 1-5.");
        return;
      }
    } else {
      Serial.println("Error: Unknown property " + prop);
      return;
    }
    Config::_saveConfig();

    return;
  }

  Serial.println("Error: unknown command config " + String(tokens[1]));
}
