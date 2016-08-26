#ifndef ESP_SWITCH_CONFIG
#define ESP_SWITCH_CONFIG
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#ifndef ESP_DEBUG
#define ESP_DEBUG(...) Serial.printf( __VA_ARGS__ )
#endif

#include "custom_config.h"

class ESPConfig
{
  public:
    ESPConfig();
    void saveConfig(ConfigData*);
    void loadConfig(ConfigData*);
    void clean();
};

extern ESPConfig espConfig;
extern ConfigData config;
#endif
