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

}

void ColorService::BeginSeasonalCycle()
{
    switch (ColorService::currentMonthOfYear)
    {
    case 1:
        BeginWinterCycle();
        break;
    case 2:
        BeginWinterCycle();
        break;
    case 3:
        if(ColorService::currentDayOfMonth == 4)
            BeginBirthdayCycle();
        else
            BeginSpringCycle();
        break;
    case 4:
        BeginSpringCycle();
        break;
    case 5:
        BeginSpringCycle();
        break;
    case 6:
        if(ColorService::currentDayOfMonth == 24)
            BeginBirthdayCycle();
        else
            BeginSummerCycle();
        break;
    case 7:
        BeginSummerCycle();
        break;
    case 8:
        BeginSummerCycle();
        break;
    case 9:
        BeginAutumnCycle();
        break;
    case 10:
        BeginHalloweenCycle();
        break;
    case 11:
        BeginAutumnCycle();
        break;
    case 12:
        BeginChristmasCycle();
        break;
    
    default:
        break;
    }
}

void ColorService::BeginChristmasCycle()
{
    for(int currentLight = 0; currentLight < 3; currentLight++)
    {
        for(int pwmValue = 0; pwmValue < 255; pwmValue++)
        {
            if(currentLight == 0)
            {
                SetColorRgb(pwmValue, 0, 0);
            }
            else if(currentLight == 1)
            {
                SetColorRgb(0, pwmValue, 0);
            }
            else
            {
                SetColorRgb(0, 0, pwmValue);
            }

            delay(15);

        }
        
        for(int pwmValue = 255; pwmValue > 0; pwmValue--)
        {
            if(currentLight == 0)
            {
                SetColorRgb(pwmValue, 0, 0);
            }
            else if(currentLight == 1)
            {
                SetColorRgb(0, pwmValue, 0);
            }
            else
            {
                SetColorRgb(0, 0, pwmValue);
            }

            delay(15);

        }
    }
}

void ColorService::BeginHalloweenCycle()
{
    int pwmMinValue = 0;
    int pwmMaxValue = 50;
    int faktorDifference = 5;

    for(int pwmValue = pwmMinValue; pwmValue < pwmMaxValue; pwmValue++)
    {
        SetColorRgb(pwmValue * faktorDifference, pwmValue, 0);
        delay(50);
    }

    for(int pwmValue = pwmMaxValue; pwmValue > pwmMinValue; pwmValue--)
    {
        SetColorRgb(pwmValue * faktorDifference, pwmValue, 0);
        delay(50);
    }
}

void ColorService::BeginBirthdayCycle()
{
    for(int pwmValue = 0; pwmValue < 255; pwmValue++)
    {
        SetColorRgb(255, pwmValue, pwmValue);
        delay(15);
    }

    for(int pwmValue = 255; pwmValue > 0; pwmValue--)
    {
        SetColorRgb(255, pwmValue, pwmValue);
        delay(15);
    }
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
    int pwmMinValue = 15;
    int pwmMaxValue = 200;

    for(int pwmValue = pwmMinValue; pwmValue < pwmMaxValue; pwmValue++)
    {
        SetColorRgb(243, pwmValue, 0);
        delay(35);
    }

    for(int pwmValue = pwmMaxValue; pwmValue > pwmMinValue; pwmValue--)
    {
        SetColorRgb(243, pwmValue, 0);
        delay(35);
    }    
}

void ColorService::BeginSpringCycle()
{
    int pwmMinValue = 0;
    int pwmMaxValue = 250;

    for(int pwmValue = pwmMinValue; pwmValue < pwmMaxValue; pwmValue++)
    {
        SetColorRgb(pwmValue, 250, 0);
        delay(35);
    }

    for(int pwmValue = pwmMaxValue; pwmValue > pwmMinValue; pwmValue--)
    {
        SetColorRgb(pwmValue, 250, 0);
        delay(35);
    }  
}

void ColorService::BeginAutumnCycle()
{
    int pwmMinValue = 0;
    int pwmMaxValue = 100;

    for(int pwmValue = pwmMinValue; pwmValue < pwmMaxValue; pwmValue++)
    {
        SetColorRgb(250, pwmValue, 0);
        delay(35);
    }

    for(int pwmValue = pwmMaxValue; pwmValue > pwmMinValue; pwmValue--)
    {
        SetColorRgb(250, pwmValue, 0);
        delay(35);
    }  
}

void ColorService::BeginArcaneCycle()
{
    int pwmMinValue = 100;
    int pwmMaxValue = 170;

    for(int pwmValue = pwmMinValue; pwmValue < pwmMaxValue; pwmValue++)
    {
        SetColorRgb(pwmValue, 0, 255);
        delay(35);
    }

    for(int pwmValue = pwmMaxValue; pwmValue > pwmMinValue; pwmValue--)
    {
        SetColorRgb(pwmValue, 0, 255);
        delay(35);
    }  

}
