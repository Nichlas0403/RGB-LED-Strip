#ifndef ColorService_h
#define ColorService_h
#include "Arduino.h"

class ColorService
{
    private:
        int R_gpio;
        int G_gpio;
        int B_gpio;
        void SetColorRgb(unsigned int red, unsigned int green, unsigned int blue);

        // void BeginSpringCycle();
        // void BeginSummerCycle();
        // void BeginAutumnCycle();
        // void BeginWinterCycle();
        // void BeginBirthdayCycle();
        // void BeginChristmasCycle();
        // void BeginHalloweenCycle();

    public:
        ColorService(int r_gpio, int g_gpio, int b_gpio);
        void ResetColors();
        unsigned int CustomColorCycleRed;
        unsigned int CustomColorCycleGreen;
        unsigned int CustomColorCycleBlue;

        void BeginSeasonalCycle(int day, int month);
        void BeginRainbowCycle();
        void BeginCustomColorCycle();
        void BeginWinterCycle();
        void BeginSummerCycle();
        void BeginSpringCycle();
        void BeginAutumnCycle();
        void BeginChristmasCycle();
        void BeginBirthdayCycle();
        void BeginHalloweenCycle();
        void BeginArcaneCycle();
};


#endif