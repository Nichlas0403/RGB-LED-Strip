#include <Arduino.h>
#include "ColorService.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#define r D2
#define g D1
#define b D0



String photoresistorPath = cscsIp + "/photoresistor-value";
String currentDateTime = cscsIp + "/current-datetime";
ESP8266WebServer _server(80);

//Core server functionality
void restServerRouting();
void connectToWiFi();

//Services
int ColorOptionSelected = 2; 
const int RainbowCycleOption = 0;
const int CustomColorCycleOption = 1;
const int SeasonalColorCycleOption = 2;
ColorService colorService(r, g, b);


void setup() {
  colorService.ResetColors();
  Serial.begin(9600);
  connectToWiFi();
  String currentDate = "2023-01-23T21:07:33.803159+01:00";
  colorService.currentMonthOfYear = int((currentDate[5] + currentDate[6]));
  colorService.currentDayOfMonth = int(currentDate[8] + currentDate[9]);

}

void loop() {
  _server.handleClient();

  switch(ColorOptionSelected)
  {
    case RainbowCycleOption:
      colorService.BeginRainbowCycle();
      break;
    case CustomColorCycleOption:
      colorService.BeginCustomColorCycle();
      break;
    case SeasonalColorCycleOption:
      colorService.BeginArcaneCycle();
      break;
  }

}


// --------------- API ENDPOINTS ------------------

void HealthCheck()
{
  _server.send(200);
}

void BeginSeasonalColorCycle()
{
  ColorOptionSelected = SeasonalColorCycleOption;
  _server.send(200);
}

void BeginCustomColorCycle()
{

  if(!_server.hasArg("red") || !_server.hasArg("green") ||!_server.hasArg("blue"))
  {
    _server.send(400, "text/json", "One or more arguments are missing: red, green, blue");
    return;
  }


  if(_server.arg("red").toInt() < 0 || _server.arg("red").toInt() > 255)
  {
    _server.send(400, "text/json", "Argument must be between 0 and 255: red");
    return;
  }
  else if(_server.arg("green").toInt() < 0 || _server.arg("green").toInt() > 255)
  {
    _server.send(400, "text/json", "Argument must be between 0 and 255: green");
    return;
  }
  else if(_server.arg("blue").toInt() < 0 || _server.arg("blue").toInt() > 255)
  {
    _server.send(400, "text/json", "Argument must be between 0 and 255: blue");
    return;
  }

  colorService.CustomColorCycleRed = _server.arg("red").toInt();
  colorService.CustomColorCycleGreen = _server.arg("green").toInt();
  colorService.CustomColorCycleBlue = _server.arg("blue").toInt();

  ColorOptionSelected = CustomColorCycleOption;

  _server.send(200);

}

// Core server functionality
void restServerRouting() 
{
  _server.on(F("/health-check"), HTTP_GET, HealthCheck);
  _server.on(F("/begin-custom-color-cycle"), HTTP_PUT, BeginCustomColorCycle);
  _server.on(F("/begin-seasonal-color-cycle"), HTTP_PUT, BeginSeasonalColorCycle);
}

void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += _server.uri();
  message += "_server: ";
  message += (_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += _server.args();
  message += "\n";

  for (uint8_t i = 0; i < _server.args(); i++) 
  {
    message += " " + _server.argName(i) + ": " + _server.arg(i) + "\n";
  }

  _server.send(404, "text/plain", message);
}

void connectToWiFi()
{
  WiFi.mode(WIFI_STA);

  if (WiFi.SSID() != _wifiName) 
  {
    Serial.println("Creating new connection to wifi");
    WiFi.begin(_wifiName, _wifiPassword);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
  }
  else
  {
    Serial.println("Using existing wifi settings...");
  }

 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(_wifiName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  _server.onNotFound(handleNotFound);
  // Start server
  _server.begin();

  Serial.println("HTTP server started");
}