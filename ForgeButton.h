/*
Forge Menu for you peons
*/
#ifndef ForgeButton_h
#define ForgeButton_h

#include "Arduino.h"

class ForgeButton {
  public:
    ForgeButton(int pin);
    
    bool Pressed();
  private:
    int _pin;
    unsigned long _delay;
    unsigned long _lastdebounce;
   int _state;
   int _lstate;
};
#endif