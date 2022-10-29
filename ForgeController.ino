#include "Arduino.h"
#include <Wire.h>
#include "FButton.h"
#include "FMenu.h"
#include "FKtype.h"
#include "FFans.h"

#define LEFT 1
#define SELECT 2
#define RIGHT 3
#define NOBUTTON 0


#define MFAN 0
#define MTEMP 1


FButton leftB(11);
FButton selectB(12);
FButton rightB(13);

bool selectedtoggle = false;
int currentpwm = 0;
int prevpwm = 0;
int desiredtemp = 0;
int prevtemp = 0;
FMenu menu;
FKtype tsensor;
FFans fans;
int menupos;
unsigned long previoustime = millis();
int buttonpressed;
void setup() {
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
  double actualtemp = tsensor.ReadC();
  menu.Temp(actualtemp);
  double diftemp = 0;
  if (currenttime - previoustime > 300) {
    if (fans.FanRunning1()) {
      menu.FanSpeed(fans.CalcRPM1());
    }
    diftemp = actualtemp - prevtemp;

    previoustime = currenttime;
    if (menupos == MTEMP && selectedtoggle == true) {

      if (actualtemp > desiredtemp + 10) {
        currentpwm = 0;

      } else if (actualtemp >= desiredtemp) {
        currentpwm /= 2;
      } else {
        if (currentpwm == 1000) {
          currentpwm = 0;
        } else if (currentpwm < 1000 && actualtemp < 5 * desiredtemp / 6) {
          currentpwm++;
        } else if (actualtemp < desiredtemp && diftemp > 1) {
          currentpwm--;

        } else if (prevpwm > currentpwm && diftemp < -1) {
          currentpwm /= 2;
        }

        if (currentpwm < 0) {
          currentpwm = 0;
        }
      }
      fans.SetPWM0(currentpwm);
      fans.SetPWM1(currentpwm);
      menu.CtrlFanSpeed(currentpwm);
    }
    prevpwm = currentpwm;
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