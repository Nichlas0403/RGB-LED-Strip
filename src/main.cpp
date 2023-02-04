#include <Arduino.h>
#include "ColorService.h"
#include "HttpService.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#define r D5
#define g D2
#define b D1



int photoresistorThreshold = 700;

ESP8266WebServer _server(80);

//Core server functionality
void restServerRouting();
void connectToWiFi();

//Services
int ColorOptionSelected = 3; 
const int CustomColorCycleOption = 0;
const int RainbowColorCycleOption = 1;
const int ArcaneColorCycleOption = 2;
const int SeasonalColorCycleOption = 3;
const int SummerColorCycleOption = 4;
const int AutumnColorCycleOption = 5;
const int WinterColorCycleOption = 6;
const int SpringColorCycleOption = 7;
const int HalloweenColorCycleOption = 8;
const int ChristmasColorCycleOption = 9;
const int BirthdayColorCycleOption = 10;

ColorService _colorService(r, g, b);
HttpService _client;


void setup() {

  Serial.begin(9600);
  _colorService.ResetColors();
  connectToWiFi();
  int photoresistorValue = (_client.GetPhotoresitorValue()).toInt();
  Serial.println(photoresistorValue);

  if(photoresistorValue > photoresistorThreshold)
  {
    Serial.println("SLEEP");
    ESP.deepSleep(1.8e9);
  }

  String currentDateTime = _client.GetCurrentDate();

  if(currentDateTime.substring(0,5) == "Error")
  {
    //This error sometimes happens. Simply needs to retry
    ESP.deepSleep(1e6);
  }

  _colorService.currentMonthOfYear = currentDateTime.substring(5,7).toInt();
  _colorService.currentDayOfMonth = currentDateTime.substring(8,10).toInt();
}

void loop() {
  _server.handleClient();

  switch(ColorOptionSelected)
  {
    case CustomColorCycleOption:
      _colorService.BeginCustomColorCycle();
      break;
    case RainbowColorCycleOption:
      _colorService.BeginRainbowCycle();
      break;
    case ArcaneColorCycleOption:
      _colorService.BeginArcaneCycle();
      break;
    case SeasonalColorCycleOption:
      _colorService.BeginSeasonalCycle();
      break;
    case SummerColorCycleOption:
      _colorService.BeginSummerCycle();
      break;
    case AutumnColorCycleOption:
      _colorService.BeginSummerCycle();
      break;
    case WinterColorCycleOption:
      _colorService.BeginWinterCycle();
      break;
    case SpringColorCycleOption:
      _colorService.BeginSpringCycle();
      break;
    case HalloweenColorCycleOption:
      _colorService.BeginHalloweenCycle();
      break;
    case ChristmasColorCycleOption:
      _colorService.BeginChristmasCycle();
      break;
    case BirthdayColorCycleOption:
      _colorService.BeginBirthdayCycle();
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

  _colorService.CustomColorCycleRed = _server.arg("red").toInt();
  _colorService.CustomColorCycleGreen = _server.arg("green").toInt();
  _colorService.CustomColorCycleBlue = _server.arg("blue").toInt();

  ColorOptionSelected = CustomColorCycleOption;

  _server.send(200);

}

void BeginRainbowColorCycle()
{
  ColorOptionSelected = RainbowColorCycleOption;
  _server.send(200);
}

void BeginArcaneColorCycle()
{
  ColorOptionSelected = ArcaneColorCycleOption;
  _server.send(200);
}

void BeginSummerColorCycle()
{
  ColorOptionSelected = SummerColorCycleOption;
  _server.send(200);
}

void BeginAutumnColorCycle()
{
  ColorOptionSelected = AutumnColorCycleOption;
  _server.send(200);
}

void BeginWinterColorCycle()
{
  ColorOptionSelected = WinterColorCycleOption;
  _server.send(200);
}

void BeginSpringColorCycle()
{
  ColorOptionSelected = SpringColorCycleOption;
  _server.send(200);
}

void BeginHalloweenColorCycle()
{
  ColorOptionSelected = HalloweenColorCycleOption;
  _server.send(200);
}

void BeginChristmasColorCycle()
{
  ColorOptionSelected = ChristmasColorCycleOption;
  _server.send(200);
}

void BeginBirthdayColorCycle()
{
  ColorOptionSelected = BirthdayColorCycleOption;
  _server.send(200);
}

// Core server functionality
void restServerRouting() 
{
  _server.on(F("/health-check"), HTTP_GET, HealthCheck);
  _server.on(F("/custom"), HTTP_PUT, BeginCustomColorCycle);
  _server.on(F("/seasonal"), HTTP_PUT, BeginSeasonalColorCycle);
  _server.on(F("/rainbow"), HTTP_PUT, BeginRainbowColorCycle);
  _server.on(F("/arcane"), HTTP_PUT, BeginArcaneColorCycle);
  _server.on(F("/summer"), HTTP_PUT, BeginSummerColorCycle);
  _server.on(F("/autumn"), HTTP_PUT, BeginAutumnColorCycle);
  _server.on(F("/winter"), HTTP_PUT, BeginWinterColorCycle);
  _server.on(F("/spring"), HTTP_PUT, BeginSpringColorCycle);
  _server.on(F("/halloween"), HTTP_PUT, BeginHalloweenColorCycle);
  _server.on(F("/christmas"), HTTP_PUT, BeginChristmasColorCycle);
  _server.on(F("/birthday"), HTTP_PUT, BeginBirthdayColorCycle);
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