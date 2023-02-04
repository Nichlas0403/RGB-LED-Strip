#include "HttpService.h"
#include "Arduino.h"

HttpService::HttpService() : _client(), _wifiClient()
{
    _baseUrl = "http://192.168.0.150";
    _photoresistorSubUrl = _baseUrl + "/photoresistor-value";
    _currentDateSubUrl = _baseUrl + "/current-datetime";
}

String HttpService::GetPhotoresitorValue()
{

    _client.begin(_wifiClient, _photoresistorSubUrl);

    int httpResponseCode = _client.GET();

    if (httpResponseCode > 0) {
        // Serial.print("HTTP Response code: ");
        // Serial.println(httpResponseCode);
        String payload = _client.getString();
        // Serial.println(payload);
        return payload;
      }
      else 
      {
        Serial.print("Error code: ");
        Serial.println(_client.errorToString(httpResponseCode));
        Serial.println(httpResponseCode);
        return "";
      }
}

String HttpService::GetCurrentDate()
{


    _client.begin(_wifiClient, _currentDateSubUrl);

    int httpResponseCode = _client.GET();

    if (httpResponseCode > 0) {
        // Serial.print("HTTP Response code: ");
        // Serial.println(httpResponseCode);
        String payload = _client.getString();
        // Serial.println(payload);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        return doc["formatted"];
      }
      else 
      {
        Serial.print("Error code: ");
        Serial.println(_client.errorToString(httpResponseCode));
        Serial.println(httpResponseCode);
        return "";
      }

}