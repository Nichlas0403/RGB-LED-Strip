#include "FlashService.h"
#include "Arduino.h"
#include <LittleFS.h>

void FlashService::WriteToFlash(String address, char *content)
{
    LittleFS.begin();
    File file = LittleFS.open(address, "w");
    // auto a = content;
    file.write(a.c_str);
    file.close();
    LittleFS.end();
}

String FlashService::ReadFromFlash(String address)
{
    LittleFS.begin();
    File file = LittleFS.open(address, "r");
    String content = file.readString();
    file.close();
    LittleFS.end();
    return content;
}

void FlashService::DeleteFromFlash(String address)
{
    LittleFS.begin();
    LittleFS.remove(address);
    LittleFS.end();
}