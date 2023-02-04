#ifndef ColorService_h
#define ColorService_h
#include "Arduino.h"

class ColorService
{
    private:
        int R_gpio;
        int G_gpio;
        int B_gpio;


    public:
        void SetColorRgb(unsigned int red, unsigned int green, unsigned int blue);
        int currentMonthOfYear;
        int currentDayOfMonth;
        ColorService(int r_gpio, int g_gpio, int b_gpio);
        void ResetColors();
        unsigned int CustomColorCycleRed;
        unsigned int CustomColorCycleGreen;
        unsigned int CustomColorCycleBlue;

        void BeginSeasonalCycle();
        void BeginRainbowCycle();
        void BeginCustomColorCycle();
        void BeginArcaneCycle();
        void BeginWinterCycle();
        void BeginSummerCycle();
        void BeginSpringCycle();
        void BeginAutumnCycle();
        void BeginChristmasCycle();
        void BeginBirthdayCycle();
        void BeginHalloweenCycle();
};


#endif