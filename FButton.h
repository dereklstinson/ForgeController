/*
Forge Menu for button debounce
*/
#ifndef FButton_h
#define FButton_h

#include "Arduino.h"

class FButton {
  public:
    FButton(int pin);
    bool Pressed();
  private:
    int _pin;
    unsigned long _delay;
    unsigned long _lastdebounce;
   int _state;
   int _lstate;
   int _holddelay;
};
#endif