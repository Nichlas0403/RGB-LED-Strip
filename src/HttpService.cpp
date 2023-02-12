#include "HttpService.h"
#include "Arduino.h"
#include "FlashService.h"

HttpService::HttpService() : _client(), _wifiClient()
{
  FlashService _flashService;

 _baseUrl = _flashService.ReadFromFlash("cscsBaseIp");
 _photoresistorSubUrl = _baseUrl + "/photoresistor-value";
 _currentDateSubUrl = _baseUrl + "/current-datetime";

}

String HttpService::GetPhotoresitorValue()
{

    _client.begin(_wifiClient, _photoresistorSubUrl);

    int httpResponseCode = _client.GET();

    if (httpResponseCode > 0) {
        String payload = _client.getString();
        return payload;
      }
      else 
      {
        Serial.print("Error code: ");
        Serial.println(_client.errorToString(httpResponseCode));
        Serial.println(httpResponseCode);
        return String(httpResponseCode);
      }
}

String HttpService::GetCurrentDate()
{


    _client.begin(_wifiClient, _currentDateSubUrl);

    int httpResponseCode = _client.GET();

    if (httpResponseCode > 0) {
        String response = _client.getString();
        return response;
      }
      else 
      {
        Serial.print("Error code: ");
        Serial.println(_client.errorToString(httpResponseCode));
        Serial.println(httpResponseCode);
        return "Error";
      }

}