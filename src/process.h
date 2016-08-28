#ifndef ESP_PROCESS
#define ESP_PROCESS
#include "api.h"
#include "config.h"
// //Switches
bool busy = false;
void process(){
  ESP_DEBUG("[esp-switch-core::process] DEBUG: processing = %s\n", config.deviceUUID);
  //   //here we are processing the commands & updates
  //   //this is called by the loop function
  if (api.accessToken[0] == 0){
    ESP_DEBUG("[esp-switch-core::process] DEBUG: fetching token\n");
    busy = true;
    api.fetchToken();
    busy = false;
  }else{
    if (strcmp(config.deviceUUID, "NEW")==0){
      ESP_DEBUG("[esp-switch-core::process] DEBUG: registering device\n");
      //need to call register service first and get deviceUUID from server
      busy = true;
      api.registerDevice();
      busy = false;
    }

  }
  #ifdef ESP_SWITCH_01

  #endif
  #ifdef ESP_SWITCH_02
  #endif
  #ifdef ESP_SWITCH_03
  #endif
  #ifdef ESP_SWITCH_04
  #endif
  #ifdef ESP_TEMP_HUMIDITY
  #endif
  #if !defined(ESP_SWITCH_01) && !defined(ESP_SWITCH_02) && !defined(ESP_SWITCH_03) && !defined(ESP_SWITCH_04) && !defined(ESP_TEMP_HUMIDITY)
  #endif
  delay(1000);
}
#endif
