#include "api.h"
#include "config.h"
#include "ESP8266HTTPUpdate.h"

#define ESP_DEBUG(...) Serial.printf( __VA_ARGS__ )
// #define ESP_DEBUG(...)

API::API(){
}
bool API::tokenValid() {
  if (millis() > expires) return false;
  if (accessToken[0]==0) return false;
  return true;
}
bool API::processResponse(JsonObject& json){
  return true;
}
bool API::fetchToken(){
  #ifdef ESP_API_DEBUG
  String url = "/1.0/oauth/token";
  #else
  String url = "/1.0/oauth/token";
  #endif
  String post = "grant_type=password&client_id=HomeBeeDevice&client_secret=HomeBee Device Workers&username=homebeedevice&password=H0m3b33D3v1c3";
  HTTPClient http;
  // String httpURL = String("https://") + host + String(":") + httpsPort + url;

  http.begin(host, httpsPort, url, fingerprint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(post);
  String response;
  if(httpCode > 0) {
    connectionValid = true;
    ESP_DEBUG("[esp-switch-core::fetchToken] httpCode: %d\n", httpCode);
    // std::unique_ptr<char[]> buf(new char[http.getSize()]);
    // http.getStream().readBytes(buf.get(), http.getSize());
    response = http.getString();
    ESP_DEBUG("[esp-switch-core::fetchToken] %s\n", response.c_str());
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("reading bytes");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::fetchToken] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        strcpy(accessToken, json["access_token"]);
        strcpy(refreshToken, json["refresh_token"]);
        expires = json["expires_in"];
        expires -= 10; //reduce expiry ...
        expires = millis() + expires * 1000;
        String tokens = "access_token: "+String(accessToken)+" refresh_token: "+String(refreshToken);
        ESP_DEBUG("[esp-switch-core::fetchToken] %s\n", tokens.c_str());
        return true;
      } else {
        ESP_DEBUG("[esp-switch-core::fetchToken] failed to load json config");
        return false;
      }
    }else{
      ESP_DEBUG("[esp-switch-core::fetchToken] ERROR: HTTP Code %s\n", String(httpCode).c_str());
      return false;
    }
  } else {
    connectionValid = false;
    ESP_DEBUG("[esp-switch-core::fetchToken] ERROR: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  http.end();
  return false;
};
bool API::registerDevice(void){
  #ifdef ESP_API_DEBUG
  String url = "/1.0/homebee/device/register?debug";
  #else
  String url = "/1.0/homebee/device/register";
  #endif
  String post = "deviceUUID="+String(config.deviceUUID)+"&deviceTYPE="+String(config.deviceTYPE)+"&version="+String(config.version)+"&chipId="+String(config.chipID);
  HTTPClient http;
  String httpURL = String("https://") + host + String(":") + httpsPort + url;
  http.begin(httpURL, fingerprint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer "+String(api.accessToken));
  int httpCode = http.POST(post);
  ESP_DEBUG("[esp-switch-core::register] httpCode: %d\n", httpCode);
  String response;
  if(httpCode > 0) {
    connectionValid = true;
    ESP_DEBUG("[esp-switch-core::register] httpCode: %d\n", httpCode);
    // std::unique_ptr<char[]> buf(new char[http.getSize()]);
    // http.getStream().readBytes(buf.get(), http.getSize());
    response = http.getString();
    ESP_DEBUG("[esp-switch-core::register] %s\n", response.c_str());
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("reading bytes");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::register] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        strcpy(config.deviceUUID, json["data"]["deviceUUID"]);
        strcpy(config.claimedID, json["claimedID"]);
        espConfig.saveConfig(&config);
        api.action = 0;
        return true;
      } else {
        ESP_DEBUG("[esp-switch-core::register] failed to load json config");
        return false;
      }
    }else if (httpCode == HTTP_CODE_INTERNAL_SERVER_ERROR){
      Serial.println("reading bytes");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::register ERROR:] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        if (strcmp("The access token provided is invalid.", json["error"])==0){
          //fetch token again
          fetchToken();
        }
        return false;
      } else {
        ESP_DEBUG("[esp-switch-core::register ERROR:] failed to load json config");
        return false;
      }
      return false;
    }
  } else {
    connectionValid = false;
    ESP_DEBUG("[esp-switch-core::register] ERROR: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  http.end();
  return false;
}
bool API::ping(String message){
  if (config.useSocket == 1){
    return postSocket(message);
  }
  #ifdef ESP_API_DEBUG
  String url = "/1.0/homebee/device/ping?debug";
  #else
  String url = "/1.0/homebee/device/ping";
  #endif
  String post = "deviceUUID="+String(config.deviceUUID)+"&message="+message;
  HTTPClient http;
  String httpURL = String("https://") + host + String(":") + httpsPort + url;
  http.begin(httpURL, fingerprint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer "+String(api.accessToken));
  int httpCode = http.POST(post);
  ESP_DEBUG("[esp-switch-core::ping] httpCode: %d\n", httpCode);
  String response;
  if(httpCode > 0) {
    connectionValid = true;
    // ESP_DEBUG("[esp-switch-core::ping] httpCode: %d\n", httpCode);
    // std::unique_ptr<char[]> buf(new char[http.getSize()]);
    // http.getStream().readBytes(buf.get(), http.getSize());
    response = http.getString();
    ESP_DEBUG("[esp-switch-core::ping] %s\n", response.c_str());
    if (httpCode == HTTP_CODE_OK) {
      // Serial.println("reading bytes");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      // json.prettyPrintTo(Serial);
      // ESP_DEBUG("\n");
      if (json.success()) {
        strcpy(command, json["data"]["command"]["command"]);
        if (strcmp(command, "NONE")!=0){
          strcpy(commandId, json["data"]["command"]["_id"]);
        }
        ESP_DEBUG("[esp-switch-core::ping] command: %s\n", command);
        return true;
      } else {
        ESP_DEBUG("[esp-switch-core::ping] failed to load json config");
        return false;
      }
    }else if (httpCode == HTTP_CODE_INTERNAL_SERVER_ERROR){
      ESP_DEBUG("[esp-switch-core::ping ERROR: 500] ");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::ping ERROR:] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        if (strcmp(json["error_description"], "Sorry, device not registered with HomeBee Portal") == 0){
          action = 1;
          return true;
        }else if(strcmp("The access token provided is invalid.", json["error"])==0){
          //fetch token again
          fetchToken();
        }
        return false;
      } else {
        ESP_DEBUG("[esp-switch-core::ping ERROR:] failed to load json: %s\n", response.c_str());
        return false;
      }
      return false;
    }
  }else{
    connectionValid = false;
    ESP_DEBUG("[esp-switch-core::ping] ERROR: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  http.end();
  return false;
}
bool API::commandUpdate(){
  if (strcmp(command, "NONE")==0){
    return false;
  }
  if (strcmp(commandId, "")==0){
    //nothing to update
    return true;
  }
  #ifdef ESP_API_DEBUG
  String url = "/1.0/homebee/device/device-command-update?debug=true";
  #else
  String url = "/1.0/homebee/device/device-command-update";
  #endif
  String post = "deviceUUID="+String(config.deviceUUID)+"&commandId="+String(commandId);
  HTTPClient http;
  String httpURL = String("https://") + host + String(":") + httpsPort + url;
  http.begin(httpURL, fingerprint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer "+String(api.accessToken));
  int httpCode = http.POST(post);
  ESP_DEBUG("[esp-switch-core::commandUpdate] httpCode: %d\n", httpCode);
  String response;
  if(httpCode > 0) {
    connectionValid = true;
    response = http.getString();
    ESP_DEBUG("[esp-switch-core::commandUpdate] %s\n", response.c_str());
    ESP_DEBUG("[esp-switch-core::commandUpdate] command updated: %s\n", command);
    if (httpCode == HTTP_CODE_OK) {
        strcpy(command, "NONE");
        return true;
    }else if (httpCode == HTTP_CODE_INTERNAL_SERVER_ERROR){
      ESP_DEBUG("[esp-switch-core::commandUpdate] ERROR: 500 \n");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::commandUpdate ERROR:] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        if (strcmp(json["error_description"], "Sorry, device not registered with HomeBee Portal") == 0){
          action = 1;
          return false;
        }else if(strcmp("The access token provided is invalid.", json["error"])==0){
          //fetch token again
          fetchToken();
        }
        return false;
      } else {
        ESP_DEBUG("[esp-switch-core::commandUpdate] ERROR: failed to load json: %s\n", response.c_str());
        return false;
      }
      return false;
    }
  }else{
    connectionValid = false;
    ESP_DEBUG("[esp-switch-core::commandUpdate] ERROR: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  http.end();
  return false;
}
bool API::update(){
  #ifdef ESP_API_DEBUG
  String url = "/1.0/homebee/device/update?debug=true";
  #else
  String url = "/1.0/homebee/device/update?debug=true";
  #endif
  String post = "deviceUUID="+String(config.deviceUUID)+"&version="+config.version;
  HTTPClient http;
  String httpURL = String("https://") + host + String(":") + httpsPort + url;
  strcpy(ESPhttpUpdate.deviceUUID, config.deviceUUID);
  strcpy(ESPhttpUpdate.accessToken, api.accessToken);
  ESPhttpUpdate.rebootOnUpdate(false);
  t_httpUpdate_return ret = ESPhttpUpdate.update(httpURL, config.version, fingerprint);
  switch(ret) {
    case HTTP_CODE_OK:
      ESP_DEBUG("[esp-switch-core::update] ERROR: update failed due to server error\n" );
      break;
    case HTTP_UPDATE_FAILED:
      ESP_DEBUG("[esp-switch-core::update] ERROR: update failed.\n");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[esp-switch-core::update] DEBUG: no Update.");
      break;
    case HTTP_UPDATE_OK:
      ESP_DEBUG("[esp-switch-core::update] Update successful .. new version: %s\n", ESPhttpUpdate.newVersion);
      strcpy(config.version, ESPhttpUpdate.newVersion);
      espConfig.saveConfig(&config);
      return true;
      break;
  }
  return false;
}
bool API::postSocket(String message){
  startSocket();
  String post = "deviceUUID="+String(config.deviceUUID)+"&message="+message;
  int httpCode = http.POST(post);
  ESP_DEBUG("[esp-switch-core::postSocket] httpCode: %d\n", httpCode);
  String response;
  if(httpCode > 0) {
    connectionValid = true;
    socketOpen = true;
    ESP_DEBUG("[esp-switch-core::postSocket] httpCode: %d\n", httpCode);
    // std::unique_ptr<char[]> buf(new char[http.getSize()]);
    // http.getStream().readBytes(buf.get(), http.getSize());
    response = http.getString();
    ESP_DEBUG("[esp-switch-core::postSocket] %s\n", response.c_str());
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("reading bytes");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::postSocket] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        return true;
      } else {
        ESP_DEBUG("[esp-switch-core::postSocket] failed to load json config");
        return false;
      }
    }else if (httpCode == HTTP_CODE_INTERNAL_SERVER_ERROR){
      Serial.println("reading bytes");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(response.c_str());
      if (json.success()) {
        ESP_DEBUG("[esp-switch-core::postSocket ERROR:] ");
        json.prettyPrintTo(Serial);
        ESP_DEBUG("\n");
        if (strcmp("The access token provided is invalid.", json["error"])==0){
          //fetch token again
          fetchToken();
        }
        return false;
      } else {
        ESP_DEBUG("[esp-switch-core::postSocket ERROR:] failed to load json config");
        return false;
      }
      return false;
    }
  } else {
    connectionValid = false;
    socketOpen = false;
    ESP_DEBUG("[esp-switch-core::postSocket] ERROR: %s\n", http.errorToString(httpCode).c_str());
    return false;
  }
  return false;
}
bool API::startSocket(){
  if (!socketOpen){
    #ifdef ESP_API_DEBUG
    String url = "/1.0/homebee/device/ping?debug";
    #else
    String url = "/1.0/homebee/device/ping";
    #endif
    String httpURL = String("https://") + host + String(":") + httpsPort + url;
    http.begin(httpURL, fingerprint);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", "Bearer "+String(api.accessToken));
    ESP_DEBUG("[esp-switch-core::endSocket] DEBUG: Socket Openned\n");
    socketOpen = true;
  }
  return true;
}
void API::endSocket(){
  ESP_DEBUG("[esp-switch-core::endSocket] DEBUG: Socket Closed\n");
  http.end();
  socketOpen = false;
}
API api;
