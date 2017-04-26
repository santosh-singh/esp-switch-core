#include "config.h"

#define ESP_DEBUG(...) Serial.printf( __VA_ARGS__ )
// #define ESP_DEBUG(...)

ESPConfig::ESPConfig(){
  Serial.println("config initialised");
}

void ESPConfig::loadConfig(ConfigData* config){
  ESP_DEBUG("[ESPConfig::loadConfig] mounting the file system\n");
  ESP_DEBUG("CHIPID: %s\n", String(ESP.getChipId()).c_str());
  if (SPIFFS.begin()) {
    ESP_DEBUG("[ESPConfig::loadConfig] file system mounted\n");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      ESP_DEBUG("[ESPConfig::loadConfig] reading config file\n");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        ESP_DEBUG("[ESPConfig::loadConfig] data loaded: ");
        json.printTo(Serial);
        if (json.success()) {
          ESP_DEBUG("[ESPConfig::loadConfig] parsed json\n");
          strcpy(config->deviceUUID, json["device_uuid"]);
          strcpy(config->deviceTYPE, json["device_type"]);
          strcpy(config->version, json["version"]);
          strcpy(config->claimedID, json["claimed_id"]);
          // strcpy(config->chipID, String(ESP.getChipId()).c_str());
          config->useSocket = json["use_socket"];
        } else {
          ESP_DEBUG("[ESPConfig::loadConfig] ERROR: failed to load json config");
          strcpy(config->chipID, String(ESP.getChipId()).c_str());
        }
      }else{
        ESP_DEBUG("[ESPConfig::loadConfig] No config file found ... loading new one");
        strcpy(config->chipID, String(ESP.getChipId()).c_str());
      }
    }else{
      strcpy(config->chipID, String(ESP.getChipId()).c_str());
    }
  } else {
    ESP_DEBUG("[ESPConfig::loadConfig] ERROR: failed to mount FS\n");
  }
}
void ESPConfig::saveConfig(ConfigData* config){
  ESP_DEBUG("[ESPConfig::loadConfig] mounting the file system\n");
  if (SPIFFS.begin()) {
    ESP_DEBUG("[ESPConfig::loadConfig] file system mounted\n");
    ESP_DEBUG("[ESPConfig::saveConfig] saving config\n");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["device_uuid"] = config->deviceUUID;
    json["device_type"] = config->deviceTYPE;
    json["version"] = config->version;
    json["claimed_id"] = config->claimedID;
    json["use_socket"] = config->useSocket;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      ESP_DEBUG("[ESPConfig::saveConfig] ERROR: failed to open config file for writing\n");
    }else{
      String jsonString;
      json.prettyPrintTo(jsonString);
      ESP_DEBUG("[ESPConfig::saveConfig] %s\n", jsonString.c_str());
      json.printTo(configFile);
      configFile.close();
      ESP_DEBUG("[ESPConfig::saveConfig] saved config\n");
    }
  }
}
void ESPConfig::clean(){
  if (SPIFFS.begin()) {
    ESP_DEBUG("[ESPConfig::loadConfig] formatting the file system\n");
    SPIFFS.format();
  }
}

ESPConfig espConfig;
ConfigData config;
