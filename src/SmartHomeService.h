#ifndef SmarthomeService_h
#define SmarthomeService_h
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HttpClient.h>
#include <WiFiClient.h>
#include "ArduinoJson.h"

class SmartHomeService
{
    public:
        SmartHomeService();
        bool UpdateIpAddress(String ipAddress);

    private:
        HTTPClient _client;
        WiFiClient _wifiClient;
};

#endif