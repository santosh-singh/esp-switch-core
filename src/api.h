#ifndef ESP_API
#define ESP_API
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#ifndef ESP_DEBUG
#define ESP_DEBUG(...) Serial.printf( __VA_ARGS__ )
#endif
class API
{
  public:
    API();
    // APIResponse response;
    bool post(String, String, String);
    bool fetchToken(void);
    bool registerDevice(void);
    char accessToken[41] = {0}; //a203fc06aa6c25ee71f24cf9d9661fc2024b22c5
    char refreshToken[41] = {0};
  private:
    const char* host = "37.188.116.81";
    const char* ssl_host = "www.surya-solutions.com";
    const int httpsPort = 4043;
    const int httpPort = 4000;
    // SHA1 fingerprint of the certificate
    const char* fingerprint = "75:FB:75:A9:20:3D:16:C7:D6:89:58:01:DD:0F:51:9D:1C:ED:0E:5F";

};
extern API api;
#endif
