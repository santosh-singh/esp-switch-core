Each device will have its own unique id ... chipid ... ESP.getChipId()
setup()
  When the device is powered on it will check for the ssid password in EEPROM ... and try to connect
  If connected it will resume client mode ... else ... AP mode
    Client mode:
      get deviceid from EEPROM address 100 - 150
      if device does not exist then this device is not registered online
      Check if not device is registered online? /api/1.0/register?deviceid=chipid = returns new deviceid  
      store the deviceid in EEPROM address 100 - 150
      now loop ...
    AP Mode:
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP( ssid.c_str() );
      Launch Web Server & DNS Server
      display page with SSID list and ask to select one with password.
      attempt to connect as WIFI_STA
      if successful, ESP.restart() else WIFI_display page again ... 
