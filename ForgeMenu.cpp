#include "Arduino.h"
#include "ForgeMenu.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

ForgeMenu::ForgeMenu() {

  lcd.init();       //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor(0, 0);
  lcd.print("  Forge Controller  ");
  lcd.setCursor(0, 1);
  lcd.print(" FAN  :D(   . )     ");
  lcd.setCursor(0, 2);
  lcd.print(" TMP  :T(    )    . ");
  lcd.setCursor(0, 3);
  lcd.print(" PSE  :             ");
}

void ForgeMenu::Temp(int val1, int val2) {
  lcd.setCursor(14, 2);
  if (val1 > 1350) {
    lcd.print("over");
    lcd.setCursor(19, 2);
    lcd.print(" ");
    return;
  }
  lcd.print(val1);
  lcd.setCursor(19, 2);
  lcd.print(val2);
}
void ForgeMenu::FanSpeed(int val) {
  lcd.setCursor(16, 1);
  lcd.print(val);
}
void ForgeMenu::SelectFan() {
  lcd.setCursor(0, 1);
  lcd.print(star);
  lcd.setCursor(4, 1);
  lcd.print(star);
  lcd.setCursor(0, 2);
  lcd.print(space);
  lcd.setCursor(4, 2);
  lcd.print(space);
  lcd.setCursor(0, 3);
  lcd.print(space);
  lcd.setCursor(4, 3);
  lcd.print(space);
}
void ForgeMenu::SelectTemp() {
  lcd.setCursor(0, 1);
  lcd.print(space);
  lcd.setCursor(4, 1);
  lcd.print(space);
  lcd.setCursor(0, 2);
  lcd.print(star);
  lcd.setCursor(4, 2);
  lcd.print(star);
  lcd.setCursor(0, 3);
  lcd.print(space);
  lcd.setCursor(4, 3);
  lcd.print(space);
}
void ForgeMenu::SelectPause() {
  lcd.setCursor(0, 1);
  lcd.print(space);
  lcd.setCursor(4, 1);
  lcd.print(space);
  lcd.setCursor(0, 2);
  lcd.print(space);
  lcd.setCursor(4, 2);
  lcd.print(space);
  lcd.setCursor(0, 3);
  lcd.print(star);
  lcd.setCursor(4, 3);
  lcd.print(star);
}
void ForgeMenu::CtrlFanSpeed(int val1, int val2) {
  lcd.setCursor(8, 1);
  lcd.print(val1);
  lcd.setCursor(12, 1);
  lcd.print(val2);
}
void ForgeMenu::CtrlTemperature(int val) {
  lcd.setCursor(8, 2);
  lcd.print(val);
}