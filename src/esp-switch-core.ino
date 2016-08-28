#define ESP_SWITCH_01
#define ESP_API_DEBUG
#define DEBUG_ESP_PORT Serial
#define DEBUG_ESP_CORE
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include "config.h"
#include "api.h"
#include "process.h"
//define your default values here, if there are different values in config.json, they are overwritten.
char homebee_server[40] = "https://37.188.116.81:3043/1.0/";

//declared outside setup to have access across the application
HTTPClient http;
WiFiManager wifiManager;

//callback notifying us of the need to save config
void connectionCallback () {
  ESP_DEBUG("[esp-switch-core::setup] Connected successfully\n");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // espConfig.saveConfig(&config);
  espConfig.loadConfig(&config);
  //Local intialization. Once its business is done, there is no need to keep it around
  //set config save notify callback
  if (!wifiManager.autoConnect()) {
    ESP_DEBUG("[esp-switch-core::setup] ERROR: failed to connect and hit timeout\n");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  ESP_DEBUG("[esp-switch-core::setup] local ip: %s\n",WiFi.localIP().toString().c_str());
}
void loop() {
  process();
}
