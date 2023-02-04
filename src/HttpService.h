#ifndef HttpService_h
#define HttpService_h
#include "Arduino.h"
#include <ESP8266HttpClient.h>
#include "ArduinoJson.h"

class HttpService
{
    private:
        WiFiClient _wifiClient;
        HTTPClient _client;

        String _baseUrl;
        String _photoresistorSubUrl;
        String _currentDateSubUrl;

    public:
        HttpService();
        String GetPhotoresitorValue();
        String GetCurrentDate();
};

#endif
