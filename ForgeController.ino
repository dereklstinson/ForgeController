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
#define MAXPWM 320
#define BASEPWM MAXPWM / 10
#define PWMOFFSET MAXPWM / 10
//#define TEMPSTEPS 50
//#define NTEMPS 1300 / TEMPSTEPS
#define MFAN 0
#define MTEMP 1

//Buttons
FButton leftB(11);
FButton selectB(12);
FButton rightB(13);

//double temps[NTEMPS];
bool selectedtoggle = false;
unsigned long currentpwm = 0;
unsigned long averagefanspeed = 0;
double currenttemp = 0.0;
double desiredtemp = 0.0;
double previoustemp = 0.0;

//Menu
FMenu menu;
//K type sensor
FKtype tsensor;
FFans fans;
//position in menu
int menupos;
unsigned long previoustime = millis();
unsigned long previoustime2 = previoustime;
unsigned long measuretemptime = previoustime;
unsigned long fanprevioustime = 0;
int buttonpressed;


void setup() {
  randomSeed(analogRead(0));
  fans.Begin(MAXPWM, 600);
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
    unsigned long fancalc = 0;
    if (fans.FanRunning1()) {
      fancalc = fans.CalcRPM1();
      if (fancalc > 9999) {
        fancalc = averagefanspeed;
      }
    }

    averagefanspeed = (2 * averagefanspeed + fancalc) / 3;
    currenttemp = (5*currenttemp +  tsensor.ReadC())/6;
    
    if (menupos == MTEMP && selectedtoggle == true) {


      if (currenttemp < desiredtemp) {
        double tempratio = ((desiredtemp - currenttemp) / desiredtemp);
        unsigned long int miloffset = (unsigned long int)(10 / tempratio);
     
        if ((currenttime - previoustime2 > 250 * miloffset) && (previoustemp >= currenttemp)) {
          unsigned long int adjustedpwm = (unsigned long int)(((double)(PWMOFFSET))*tempratio);
          if (adjustedpwm==0){
            adjustedpwm=1;
          }
          unsigned long int minpwmval = currentpwm - adjustedpwm;
          unsigned long int maxpwmval= currentpwm + adjustedpwm;
          if (minpwmval <= BASEPWM) {
        minpwmval=BASEPWM;
          }
          if (maxpwmval >= MAXPWM) {
            maxpwmval = MAXPWM;
          } 
          
          currentpwm = random(minpwmval, maxpwmval);
          previoustemp = currenttemp;
          previoustime2 = currenttime;
        }

      } else {
        currentpwm = BASEPWM;
      }
      if (currentpwm > MAXPWM) {
        currentpwm = MAXPWM;
      }
      if (currentpwm < BASEPWM) {
        currentpwm = BASEPWM;
      }
      fans.SetPWM0(currentpwm);
      fans.SetPWM1(currentpwm);
    }
    if (currenttemp == 0) {
      currenttemp = tsensor.ReadC();
    }
    menu.FanSpeed(averagefanspeed);

    menu.CtrlFanSpeed(currentpwm);
    menu.Temp(currenttemp);

    previoustime = currenttime;
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
              if (currentpwm > 0 && currentpwm <= MAXPWM) {
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
              if (currentpwm >= 0 && currentpwm < MAXPWM) {
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