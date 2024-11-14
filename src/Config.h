#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

#define MAX_CMD_TOKENS 8  // Maximum number of tokens to parse

class Config {
public:
  Config();   
  void setup();        
  String getWifiSSID();      
  String getWifiPassword();  
  String getHomeAssistantUrl();     
  String getHomeAssistantToken();     
  String getCoverEntityId(int index); 
  String getBatteryEntityId(int index); 
  void readCommand();

private:
  Preferences _preferences;
  String _wifiSSID;   
  String _wifiPassword;  
  String _homeAssistantUrl;   
  String _homeAssistantToken;     
  String _coverEntityId[5];
  String _batteryEntityId[5];
  void _saveConfig();
  void _runHelp(String tokens[MAX_CMD_TOKENS], int tokenCount);
  void _runConfig(String tokens[MAX_CMD_TOKENS], int tokenCount);
};
#endif