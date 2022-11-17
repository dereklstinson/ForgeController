/*
Forge Fan controls
*/
#ifndef FFans_h
#define FFans_h

#include "Arduino.h"

class FFans {
  public:
    FFans();
    //This will calculate RPM based soley on time between interupts.
    //Problem is that it can't calculate it when the fan isn't running
    void Begin();
    unsigned long CalcRPM0();
        //0 to 1000
    void SetPWM0(unsigned long int b);
    unsigned long CalcRPM1();
        //0 to 1000
     void SetPWM1(unsigned long int b);

  
    //This checks if the fan is running
    bool FanRunning0();
    bool  FanRunning1();
  private:
unsigned long volatile globaltime2 = 0;
unsigned long volatile globaltime3 = 0;
};
#endif