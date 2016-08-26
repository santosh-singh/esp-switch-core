#ifndef ESP_PROCESS
#define ESP_PROCESS
#include "api.h"
#include "config.h"
// //Switches
void process(){
//   //here we are processing the commands & updates
//   //this is called by the loop function
  if (api.accessToken == ""){
    api.fetchToken();
  }
  if (config.deviceUUID == "NEW"){
    //need to call register service first and get deviceUUID from server
    api.registerDevice();
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
}
#endif
