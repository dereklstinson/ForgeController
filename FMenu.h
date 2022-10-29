/*

Forge Menu graphics

*/

#ifndef FMenu_h
#define FMenu_h


#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


class FMenu {

public:
  FMenu();
  void Begin();
  void Temp(int val);
  void FanSpeed(unsigned int val);
  void SelectFan();
  void SelectTemp();
 // void SelectPause();
  void Select();
 // void SelectedPause();
  void SelectedTemp();
  void SelectedFan();
  void Error(char c);
  void CtrlFanSpeed(int val1);
  void CtrlTemperature(int val);

private:

LiquidCrystal_I2C lcd=LiquidCrystal_I2C(0x27,20,4);
const char star = '*';
const char space = ' ';
};


#endif