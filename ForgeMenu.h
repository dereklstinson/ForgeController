/*

Forge Menu for you peons

*/

#ifndef ForgeMenu_h
#define ForgeMenu_h


#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


class ForgeMenu {

public:
  ForgeMenu();

  void Temp(int val1, int val2);
  void FanSpeed(int val);
  void SelectFan();
  void SelectTemp();
  void SelectPause();
  void CtrlFanSpeed(int val1,int val2);
  void CtrlTemperature(int val);

private:
LiquidCrystal_I2C lcd=LiquidCrystal_I2C(0x27,20,4);
const char star = '*';
const char space = ' ';
};


#endif