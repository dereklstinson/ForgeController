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
  void Begin( unsigned long int Maxpwmvalue=320, unsigned long int MinWaitMils = 600);
  
  unsigned long int MaxPwmValue();
  
  unsigned long CalcRPM0();
  
  void SetPWM0(unsigned long int b = 0);
  
  unsigned long CalcRPM1();

  void SetPWM1(unsigned long int b = 0);


  //This checks if the fan is running
  //mincheckmils is the minimum mount of time it you want to wait to see if the tach counter has been counting
  //lets say a fan runs at 1000 rpm. That's 16.6667 rps or 0.0166667 revolutions per millisecond
  bool FanRunning0();
  bool FanRunning1();
private:
  unsigned long volatile globaltime2 = 0;
  unsigned long volatile globaltime3 = 0;
  unsigned long volatile previoustachcount2 = 0;
  unsigned long volatile previoustachcount3 = 0;
   unsigned long int maxpwmvalue;
   unsigned long int minwaitmils;
  bool fanrunningflag0 = false;
  bool fanrunningflag1 = false;
};
#endif