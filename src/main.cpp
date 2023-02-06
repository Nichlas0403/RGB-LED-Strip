#include <Arduino.h>
#include "ColorService.h"
#include "HttpService.h"
#include "FlashService.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include <LittleFS.h>

#define r D5
#define g D2
#define b D1

String _wifiSSIDFlash = "wifiSSID";
String _wifiPasswordFlash = "wifiPassword";
String _cscsBaseIp = "cscsBaseIp";
String _photoResistorThresholdFlash = "photoResistorThreshold";
String _colorOptionSelectedFlash = "colorOptionSelected";
String _customColorRedFlash = "customColorRedFlash";
String _customColorGreenFlash = "customColorGreenFlash";
String _customColorBlueFlash = "customColorBlueFlash";

int _photoresistorThreshold;
String _wifiName;
String _wifiPassword;

ESP8266WebServer _server(80);

//Core server functionality
void restServerRouting();
void connectToWiFi();

//Services
volatile int ColorOptionSelected;
const int RainbowColorCycleOption = 0;
const int CustomColorCycleOption = 1;
const int ArcaneColorCycleOption = 2;
const int SeasonalColorCycleOption = 3;
const int SummerColorCycleOption = 4;
const int AutumnColorCycleOption = 5;
const int WinterColorCycleOption = 6;
const int SpringColorCycleOption = 7;
const int HalloweenColorCycleOption = 8;
const int ChristmasColorCycleOption = 9;
const int BirthdayColorCycleOption = 10;

int CustomColorRed;
int CustomColorGreen;
int CustomColorBlue;

ColorService _colorService(r, g, b);
HttpService _client;
FlashService _flashService;

void setup() {

  Serial.begin(115200);

  _photoresistorThreshold = (_flashService.ReadFromFlash(_photoResistorThresholdFlash)).toInt();
  _wifiName = _flashService.ReadFromFlash(_wifiSSIDFlash);
  _wifiPassword = _flashService.ReadFromFlash(_wifiPasswordFlash);
  ColorOptionSelected = (_flashService.ReadFromFlash(_colorOptionSelectedFlash)).toInt();

  CustomColorRed = (_flashService.ReadFromFlash(_customColorRedFlash)).toInt();
  CustomColorGreen = (_flashService.ReadFromFlash(_customColorGreenFlash)).toInt();
  CustomColorBlue = (_flashService.ReadFromFlash(_customColorBlueFlash)).toInt();

  _colorService.CustomColorCycleRed = CustomColorRed;
  _colorService.CustomColorCycleGreen = CustomColorGreen;
  _colorService.CustomColorCycleBlue = CustomColorBlue;

  _colorService.ResetColors();

  connectToWiFi();

  int photoresistorValue = (_client.GetPhotoresitorValue()).toInt();

  if(photoresistorValue > _photoresistorThreshold)
  {
    Serial.println("SLEEP");
    ESP.deepSleep(1.8e9);
  }

  String currentDateTime = _client.GetCurrentDate();

  if(currentDateTime.substring(0,5) == "Error")
  {
    //An error sometimes returned by the server. Need to retry
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

void GetPhotoresistorThreshold()
{

  String threshold = _flashService.ReadFromFlash(_photoResistorThresholdFlash);
  _server.send(200, "text/json", threshold);

}

void SetPhotoresistorThreshold()
{
  String arg = "threshold";

  if(!_server.hasArg(arg))
  {
    _server.send(400, "text/json", "Argument: threshold is missing");
    return;
  }

  int threshold = (_server.arg(arg)).toInt();

  if(threshold < 0 || threshold > 1023)
  {
    _server.send(400, "text/json", "Value must be between 0 and 1023");
  }

  _flashService.WriteToFlash(_photoResistorThresholdFlash, String(threshold));

  _server.send(200);

}

void SetWifiName()
{
  String arg = "wifiName";

  if(!_server.hasArg(arg))
  {
    _server.send(400, "text/json", "Argument: wifiName is missing");
    return;
  }

  _flashService.WriteToFlash(_wifiSSIDFlash, _server.arg(arg));

  _server.send(200);

}

void SetWifiPassword()
{
  String arg = "wifiPassword";

  if(!_server.hasArg(arg))
  {
    _server.send(400, "text/json", "Argument: wifiPassword is missing");
    return;
  }

  _flashService.WriteToFlash(_wifiPassword, _server.arg(arg));

  _server.send(200);

}

void SetCSCSBaseIp()
{
  String arg = "ip";

  if(!_server.hasArg(arg))
  {
    _server.send(400, "text/json", "Argument: ip is missing");
    return;
  }

  _flashService.WriteToFlash(_cscsBaseIp, _server.arg(arg));

  _server.send(200);

}

void BeginSeasonalColorCycle()
{
  ColorOptionSelected = SeasonalColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
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

  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
  _flashService.WriteToFlash(_customColorRedFlash, String(_colorService.CustomColorCycleRed));
  _flashService.WriteToFlash(_customColorBlueFlash, String(_colorService.CustomColorCycleBlue));
  _flashService.WriteToFlash(_customColorGreenFlash, String(_colorService.CustomColorCycleGreen));

}

void BeginRainbowColorCycle()
{
  ColorOptionSelected = RainbowColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginArcaneColorCycle()
{
  ColorOptionSelected = ArcaneColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginSummerColorCycle()
{
  ColorOptionSelected = SummerColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginAutumnColorCycle()
{
  ColorOptionSelected = AutumnColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginWinterColorCycle()
{
  ColorOptionSelected = WinterColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginSpringColorCycle()
{
  ColorOptionSelected = SpringColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginHalloweenColorCycle()
{
  ColorOptionSelected = HalloweenColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginChristmasColorCycle()
{
  ColorOptionSelected = ChristmasColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

void BeginBirthdayColorCycle()
{
  ColorOptionSelected = BirthdayColorCycleOption;
  _server.send(200);
  _flashService.WriteToFlash(_colorOptionSelectedFlash, String(ColorOptionSelected));
}

// Core server functionality
void restServerRouting() 
{
  _server.on(F("/health-check"), HTTP_GET, HealthCheck);
  _server.on(F("/photoresistor-threshold"), HTTP_GET, GetPhotoresistorThreshold);
  _server.on(F("/photoresistor-threshold"), HTTP_PUT, SetPhotoresistorThreshold);
  _server.on(F("/wifi-name"), HTTP_PUT, SetWifiName);
  _server.on(F("/wifi-password"), HTTP_PUT, SetWifiPassword);
  _server.on(F("/cscs-baseip"), HTTP_PUT, SetCSCSBaseIp);
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