#ifndef ESP_API
#define ESP_API
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

class API
{
  public:
    API();
    // APIResponse response;
    bool post(String, String, String);
    bool fetchToken(void);
    bool registerDevice(void);
    bool ping(String);
    bool update();
    char accessToken[41] = {0}; //a203fc06aa6c25ee71f24cf9d9661fc2024b22c5
    char refreshToken[41] = {0};
    unsigned long expires;
    bool tokenValid();
    bool connectionValid;
    HTTPClient http;
    bool socketOpen;
    bool startSocket();
    void endSocket();
    bool postSocket(String message);
    char command[50] = {0};
    char commandId[50] = {'N', 'O', 'N', 'E', 0};
    bool commandUpdate();

    //0 - ping, 1 - register, 2 - reset wifi, 3 - update firmware, 4 - restart
    int action = 0;
  private:
    // const char* host = "37.188.116.81";
    // const char* ssl_host = "www.surya-solutions.com";
    // const int httpsPort = 4043;
    // const int httpPort = 4000;
    // // SHA1 fingerprint of the certificate
    // const char* fingerprint = "75:FB:75:A9:20:3D:16:C7:D6:89:58:01:DD:0F:51:9D:1C:ED:0E:5F";
    // const char* host = "192.168.0.14";
    const char* host = "192.168.1.12";
    const char* ssl_host = "www.surya-solutions.com";
    const int httpsPort = 4043;
    const int httpPort = 4000;
    // SHA1 fingerprint of the certificate
    const char* fingerprint = "75:FB:75:A9:20:3D:16:C7:D6:89:58:01:DD:0F:51:9D:1C:ED:0E:5F";
    bool processResponse(JsonObject&);
};
extern API api;
#endif
