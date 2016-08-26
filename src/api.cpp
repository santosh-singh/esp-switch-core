#include "api.h"
#include "config.h"

API::API(){
  strcpy(accessToken, "");
  strcpy(refreshToken, "");
}

bool API::fetchToken(){
  #ifdef ESP_API_DEBUG
  String url = "/1.0/oauth/token?debug";
  #else
  String url = "/1.0/oauth/token";
  #endif
  String post = "grant_type=password&client_id=HomeBeeDeviceApp&client_secret=HomeBee Workers&username=homebeedevice&password=H0m3b33D3v1c3";
  HTTPClient http;
  String httpURL = String("https://") + host + String(":") + httpsPort + url;
  http.begin(httpURL, fingerprint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(post);
  String response;
  if(httpCode > 0) {
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
          strcpy(accessToken, json["refresh_token"]);
          strcpy(refreshToken, json["refresh_token"]);
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
      ESP_DEBUG("[esp-switch-core::fetchToken] ERROR: %s\n", http.errorToString(httpCode).c_str());
      return false;
  }
  http.end();
  return false;
};

bool API::registerDevice(void){
  #ifdef ESP_API_DEBUG
  String url = "/1.0/homebee/register?debug";
  #else
  String url = "/1.0/homebee/register";
  #endif
  String post = "deviceUUID="+String(config.deviceUUID)+"&deviceTYPE="+String(config.deviceTYPE)+"&version="+String(config.version);
  HTTPClient http;
  String httpURL = String("https://") + host + String(":") + httpsPort + url;
  http.begin(httpURL, fingerprint);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer "+String(api.accessToken));
  int httpCode = http.POST(post);
  String response;
  if(httpCode > 0) {
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
          return true;
        } else {
          ESP_DEBUG("[esp-switch-core::register] failed to load json config");
          return false;
        }
      }else{
        ESP_DEBUG("[esp-switch-core::register] ERROR: HTTP Code %s\n", String(httpCode).c_str());
        return false;
      }
  } else {
      ESP_DEBUG("[esp-switch-core::register] ERROR: %s\n", http.errorToString(httpCode).c_str());
      return false;
  }
  http.end();
  return false;
}

API api;
