/*
Forge Menu for you peons
*/
#ifndef ForgeFans_h
#define ForgeFans_h

#include "Arduino.h"

class ForgeFans {
  public:
    ForgeFans();
    //This will calculate RPM based soley on time between interupts.
    //Problem is that it can't calculate it when the fan isn't running
    unsigned long CalcRPM2();
    unsigned long CalcRPM3();
    //0 to 1000
    void SetPWM9(long int b);
    //0 to 1000
    void SetPWM10(long int b);
    //This checks if the fan is running
    bool FanRunning2();
    bool  FanRunning3();
  private:
unsigned long volatile globaltime2 = 0;
unsigned long volatile globaltime3 = 0;
};
#endif