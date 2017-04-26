#ifndef ESP_PROCESS
#define ESP_PROCESS
#include "api.h"
#include "config.h"
// //Switches
bool busy = false;
int count = 1;
unsigned long updateDelay;

void process(){
  // ESP_DEBUG("[esp-switch-core::process] DEBUG: processing = %s\n", config.deviceUUID);
  //   //here we are processing the commands & updates
  //   //this is called by the loop function
  if (!api.tokenValid()){
    ESP_DEBUG("[esp-switch-core::process] DEBUG: fetching token\n");
    busy = true;
    api.fetchToken();
    busy = false;
  }
  if (strcmp(config.deviceUUID, "NEW")==0 || api.action == 1){
    ESP_DEBUG("[esp-switch-core::process] DEBUG: registering device\n");
    //need to call register service first and get deviceUUID from server
    busy = true;
    api.registerDevice();
    busy = false;
  }
  #ifdef ESP_SWITCH_01
  String message = String(count++);
  api.ping(message);
  if (!api.connectionValid){
    ESP_DEBUG("[esp-switch-core::process] ERROR: Server not reachable\n");
    //blink or flash LED
  }else{
    if (strcmp(api.command, "SOCKET")==0){
      config.useSocket = 1;
      espConfig.saveConfig(&config);
    }else if (strcmp(api.command, "NOSOCKET")==0){
      config.useSocket = 0;
      espConfig.saveConfig(&config);
    }else if (strcmp(api.command, "RESET")==0){
      api.commandUpdate();
    }else if (strcmp(api.command, "RESTART")==0){
      if (api.commandUpdate()){
        ESP.restart();
      }
    }else if (strcmp(api.command, "UPDATE")==0){
      if (api.update()){
        api.commandUpdate();
        delay(500);
        ESP.restart();
      }
    }
  }
  if (updateDelay == 0){
     updateDelay= millis();
  }else{
    if ((millis()- updateDelay) > (24*60*60*1000)){
      //time to request an update
      updateDelay= millis();
      strcpy(api.command, "UPDATE");
      strcpy(api.commandId, "");
      if (api.update()){
        api.commandUpdate();
        delay(500);
        ESP.restart();
      }
    }
  }
  #endif
  #ifdef ESP_SWITCH_02
  String message = "";
  #endif
  #ifdef ESP_SWITCH_03
  String message = "";
  #endif
  #ifdef ESP_SWITCH_04
  String message = "";
  #endif
  #ifdef ESP_TEMP_HUMIDITY
  String message = "";
  #endif
  #if !defined(ESP_SWITCH_01) && !defined(ESP_SWITCH_02) && !defined(ESP_SWITCH_03) && !defined(ESP_SWITCH_04) && !defined(ESP_TEMP_HUMIDITY)
  String message = "";
  #endif
  //ping every cycle
  delay(config.delay);
}
#endif
