#include "Arduino.h"
#include "FMenu.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

FMenu::FMenu() {
}
void FMenu::Begin() {
  lcd.init();       //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor(0, 0);
  lcd.print("*** ForgeControl ***");
 // lcd.print(" PSE  :Forge Control");
  lcd.setCursor(0, 1);
  lcd.print(" FAN  :D(    )      ");
  lcd.setCursor(0, 2);
  lcd.print(" TMP  :T(    )      ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}
void FMenu::Select() {
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("    Select/Pause    ");
}
/*
void FMenu::SelectedPause() {
  lcd.setCursor(0, 3);

  lcd.print("   Selected Pause   ");
}
*/
void FMenu::SelectedTemp() {
  lcd.setCursor(0, 3);

  lcd.print("Selected Temperature");
}
void FMenu::SelectedFan() {
  lcd.setCursor(0, 3);

  lcd.print("    Selected Fan    ");
}
void FMenu::Error(char c) {
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  lcd.print("Error: ");
  lcd.setCursor(5, 3);
  lcd.print(c);
}

void FMenu::Temp(int val) {

  if (val >= 1000) {
    lcd.setCursor(16, 2);
  } else if (val >= 100) {
    lcd.setCursor(16, 2);
  lcd.print(" ");
  } else if (val >= 10) {
  lcd.setCursor(16, 2);
  lcd.print("  ");
  } else if (val >= 0) {
  lcd.setCursor(16, 2);
  lcd.print("   ");
  }
  lcd.print(val);
}
void FMenu::FanSpeed(unsigned int val) {

  if (val >= 1000) {
    lcd.setCursor(16, 1);
  } else if (val >= 100) {
    lcd.setCursor(16, 1);
  lcd.print(" ");
  } else if (val >= 10) {
     lcd.setCursor(16, 1);
  lcd.print("  ");
  } else if (val >= 0) {
     lcd.setCursor(16, 1);
  lcd.print("   ");
  }

  lcd.print(val);
}
void FMenu::SelectFan() {

  lcd.setCursor(0, 1);
  lcd.print(star);
  lcd.setCursor(4, 1);
  lcd.print(star);
  lcd.setCursor(0, 2);
  lcd.print(space);
  lcd.setCursor(4, 2);
  lcd.print(space);
}
void FMenu::SelectTemp() {

  lcd.setCursor(0, 1);
  lcd.print(space);
  lcd.setCursor(4, 1);
  lcd.print(space);
  lcd.setCursor(0, 2);
  lcd.print(star);
  lcd.setCursor(4, 2);
  lcd.print(star);
}

void FMenu::CtrlFanSpeed(int val1) {
  // lcd.setCursor(9, 1);

  if (val1 >= 1000) {
    lcd.setCursor(9, 1);
  } else if (val1 >= 100) {
    lcd.setCursor(9, 1);
    lcd.print(" ");
    // lcd.setCursor(10, 1);
  } else if (val1 >= 10) {
    lcd.setCursor(9, 1);
    lcd.print("  ");
    //lcd.setCursor(11, 1);
  } else if (val1 >= 0) {
    lcd.setCursor(9, 1);
    lcd.print("   ");
    //  lcd.setCursor(12, 1);
  }
  lcd.print(val1);
}
void FMenu::CtrlTemperature(int val) {

  if (val >= 1000) {
    lcd.setCursor(9, 2);
  } else if (val >= 100) {
    lcd.setCursor(9, 2);
    lcd.print(" ");
  } else if (val >= 10) {
    lcd.setCursor(9, 2);
    lcd.print("  ");
  } else if (val >= 0) {
    lcd.setCursor(9, 2);
    lcd.print("   ");
  }

  lcd.print(val);
}