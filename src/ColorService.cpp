#include "ColorService.h"
#include "Arduino.h"

ColorService::ColorService(int r_gpio, int g_gpio, int b_gpio)
{
    R_gpio = r_gpio;
    G_gpio = g_gpio;
    B_gpio = b_gpio;
}

void ColorService:: ResetColors()
{
    analogWrite(R_gpio, 0);
    analogWrite(G_gpio, 0);
    analogWrite(B_gpio, 0); 
}

void ColorService::SetColorRgb(unsigned int red, unsigned int green, unsigned int blue)
{
    analogWrite(R_gpio, red);
    analogWrite(G_gpio, green);
    analogWrite(B_gpio, blue);
}

void FadeColors(unsigned int red, unsigned int green, unsigned int blue)
{
}

void ColorService::BeginRainbowCycle()
{
    unsigned int rgbColour[3];

    // Start off with red.
    rgbColour[0] = 255;
    rgbColour[1] = 0;
    rgbColour[2] = 0;  

    // Choose the colours to increment and decrement.
    for (int decColour = 0; decColour < 3; decColour += 1) {
        int incColour = decColour == 2 ? 0 : decColour + 1;

        // cross-fade the two colours.
        for(int i = 0; i < 255; i += 1) {
        rgbColour[decColour] -= 1;
        rgbColour[incColour] += 1;
        
        SetColorRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        delay(5);
        }
    }
}

void ColorService::BeginCustomColorCycle()
{
    SetColorRgb(CustomColorCycleRed, CustomColorCycleGreen, CustomColorCycleBlue);

    // if(ColorService::ColorCycleFade)
    //     FadeColors(CustomColorCycleRed, CustomColorCycleGreen, CustomColorCycleBlue);
}

void ColorService::BeginSeasonalCycle(int day, int month)
{
    BeginSummerCycle();
}

void ColorService::BeginWinterCycle()
{
    for(int pwmValue = 0; pwmValue < 255; pwmValue++)
    {
        SetColorRgb(pwmValue, pwmValue, 255);
        delay(15);
    }

    for(int pwmValue = 255; pwmValue > 0; pwmValue--)
    {
        SetColorRgb(pwmValue, pwmValue, 255);
        delay(15);
    }    
}

void ColorService::BeginSummerCycle()
{

    for(int pwmValue = 15; pwmValue < 130; pwmValue++)
    {
        SetColorRgb(243, pwmValue, 0);
        delay(35);
    }

    for(int pwmValue = 130; pwmValue > 15; pwmValue--)
    {
        SetColorRgb(243, pwmValue, 0);
        delay(35);
    }    
}
