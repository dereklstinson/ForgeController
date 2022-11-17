#include "Arduino.h"
#include <Wire.h>
#include "FButton.h"
#include "FMenu.h"
#include "FKtype.h"
#include "FFans.h"

//Menu options
#define LEFT 1
#define SELECT 2
#define RIGHT 3
#define NOBUTTON 0
#define BASEPWM 65
//#define TEMPSTEPS 50
//#define NTEMPS 1300 / TEMPSTEPS
#define MFAN 0
#define MTEMP 1

//Buttons
FButton leftB(11);
FButton selectB(12);
FButton rightB(13);

//double temps[NTEMPS];
int togglefanoffms = 6000;
int togglefanstotalms = 10000;
bool selectedtoggle = false;
unsigned long currentpwm = 0;
unsigned long averagefanspeed = 0;
double averagetemp = 0.0;
double desiredtemp = 0.0;
double previousaveragetemp = 0.0;
double baseblow = 0.0;
//double maxblow =0.0;
//Menu
FMenu menu;
//K type sensor
FKtype tsensor;
FFans fans;
//position in menu
int menupos;
unsigned long previoustime = millis();
unsigned long previoustime2 = millis();
int buttonpressed;
bool overshoot = false;

void setup() {
  randomSeed(analogRead(0));
  baseblow = random(BASEPWM, 1000);
  //for (unsigned long int i = 0; i < NTEMPS; i++) {
  //  temps[i] = (double)((i + 1) * TEMPSTEPS);
  //}
  fans.Begin();
  menu.Begin();
  if (!tsensor.Begin()) {
    menu.Error("TempSense");
  }
  selectedtoggle = false;
  menu.SelectFan();
  menu.Select();
  menupos = 0;
}

void loop() {
  unsigned long currenttime = millis();

  if (currenttime - previoustime > 250) {
    if (fans.FanRunning1()) {
      unsigned long fancalc = fans.CalcRPM1();
      if (fancalc > 9999) {
        fancalc = 9999;
      }
      averagefanspeed = (7 * averagefanspeed + fancalc) / 8;
      menu.FanSpeed(averagefanspeed);
    } else {
      menu.FanSpeed(0);
    }
    double actualtemp = tsensor.ReadC();
    averagetemp = (actualtemp * 3 + actualtemp) / 4;
    actualtemp = averagetemp;
    menu.Temp(actualtemp);

    previoustime = currenttime;
    if (menupos == MTEMP && selectedtoggle == true) {

      if (actualtemp < desiredtemp) {
        if (currenttime - previoustime2 > 5000) {

          if (previousaveragetemp < averagetemp) {
            if (overshoot == false) {
              baseblow = random(BASEPWM, 999);
            } else {
              overshoot = false;
              baseblow -= 3;
              if (baseblow < BASEPWM) {
                baseblow = BASEPWM;
              }
            }
          }
          previoustime2 = currenttime;
          previousaveragetemp = averagetemp;
        }
        currentpwm = (unsigned int)((((999.0 - baseblow) * (desiredtemp - actualtemp)) / desiredtemp) + baseblow);


      } else if (actualtemp > desiredtemp) {
        currentpwm = BASEPWM;
        overshoot = true;
      }
      if (currentpwm > 1000) {
        currentpwm = 1000;
      }
      if (currentpwm > 0 && currentpwm < BASEPWM) {
        currentpwm = BASEPWM;
      }


      fans.SetPWM0(currentpwm);
      fans.SetPWM1(currentpwm);
      menu.CtrlFanSpeed(currentpwm);
    }
  }

  if (leftB.Pressed()) {
    buttonpressed = LEFT;
  } else if (selectB.Pressed()) {
    buttonpressed = SELECT;
  } else if (rightB.Pressed()) {
    buttonpressed = RIGHT;
  } else {
    buttonpressed = NOBUTTON;
  }
  switch (selectedtoggle) {
    case true:
      switch (buttonpressed) {
        case SELECT:
          selectedtoggle = !selectedtoggle;
          menu.Select();
          fans.SetPWM0(0);
          fans.SetPWM1(0);

          menu.CtrlFanSpeed(0);
          break;
        case LEFT:
          switch (menupos) {

            case MFAN:
              if (currentpwm > 0 && currentpwm <= 1000) {
                currentpwm--;
                fans.SetPWM0(currentpwm);
                fans.SetPWM1(currentpwm);

                menu.CtrlFanSpeed(currentpwm);
              }
              break;
            case MTEMP:
              if (desiredtemp > 0 && desiredtemp <= 1350) {
                desiredtemp--;
                menu.CtrlTemperature(desiredtemp);
              }
              break;
          }
          break;

        case RIGHT:
          switch (menupos) {

            case MFAN:
              if (currentpwm >= 0 && currentpwm < 1000) {
                currentpwm++;
                fans.SetPWM0(currentpwm);
                fans.SetPWM1(currentpwm);
                menu.CtrlFanSpeed(currentpwm);
              }
              break;
            case MTEMP:
              if (desiredtemp >= 0 && desiredtemp < 1350) {
                desiredtemp++;
                menu.CtrlTemperature(desiredtemp);
              }
              break;
          }
          break;
      }

      break;
    case false:
      switch (buttonpressed) {
        case SELECT:
          selectedtoggle = !selectedtoggle;
          fans.SetPWM0(currentpwm);
          fans.SetPWM1(currentpwm);

          menu.CtrlFanSpeed(currentpwm);
          switch (menupos) {

            case MFAN:
              menu.SelectedFan();
              break;
            case MTEMP:
              menu.SelectedTemp();
              break;
          }

          break;
        case LEFT:
          switch (menupos) {

            case MFAN:
              menupos = MTEMP;
              menu.SelectTemp();
              break;
            case MTEMP:
              menupos = MFAN;
              menu.SelectFan();
              break;
          }
          break;

        case RIGHT:
          switch (menupos) {

            case MFAN:
              menupos = MTEMP;
              menu.SelectTemp();
              break;
            case MTEMP:
              menupos = MFAN;
              menu.SelectFan();
              break;
          }
          break;
      }
      break;
  }
}

// put your main code here, to run repeatedly: