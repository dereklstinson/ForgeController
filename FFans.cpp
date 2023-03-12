#include "Arduino.h"
#include "FFans.h"

unsigned long volatile tachcurrent2 = 0;
unsigned long volatile tachprevious2 = 0;
unsigned long volatile tachcount2 = 0;
unsigned long volatile tachcurrent3 = 0;
unsigned long volatile tachprevious3 = 0;
unsigned long volatile tachcount3 = 0;


void risingdiffs2() {
  tachcount2++;
  tachprevious2 = tachcurrent2;
  tachcurrent2 = micros();
}

void risingdiffs3() {
  tachcount3++;
  tachprevious3 = tachcurrent3;
  tachcurrent3 = micros();
}

FFans::FFans() {
}
void FFans::Begin(unsigned long int Maxpwmvalue, unsigned long int MinWaitMils) {
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << CS10) | (1 << WGM13);
  ICR1 = 320;
  OCR1A = 0;
  OCR1B = 0;
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), risingdiffs2, RISING);
  attachInterrupt(digitalPinToInterrupt(3), risingdiffs3, RISING);
  maxpwmvalue = Maxpwmvalue;
  minwaitmils = MinWaitMils;
}

unsigned long int FFans::MaxPwmValue() {
  return maxpwmvalue;
}
unsigned long FFans::CalcRPM0() {
  unsigned long difference = tachcurrent2 - tachprevious2;
  if (difference == 0) {
    return 0;
  }
  //two rises per rotation
  //60 seconds in a minute
  //1,000,000 microseconds in a second
  //60000000ms/minute * 1 rotation/(2*difference(microseconds))
  //30000000/difference
  return 30000000 / difference;
}
unsigned long FFans::CalcRPM1() {
  unsigned long difference = tachcurrent3 - tachprevious3;
  if (difference == 0) {
    return 0;
  }
  //two rises per rotation
  //60 seconds in a minute
  //1,000,000 microseconds in a second
  //60000000ms/minute * 1 rotation/(2*difference(microseconds))
  //30000000/difference
  return 30000000 / difference;
}
void FFans::SetPWM1(unsigned long int b) {
  if (b > maxpwmvalue) {
    b = maxpwmvalue;
  }
  unsigned long int x = (320 * b);
  OCR1A = (uint16_t)(x / maxpwmvalue);
}
void FFans::SetPWM0(unsigned long int b) {
  if (b > maxpwmvalue) {
    b = maxpwmvalue;
  }
  unsigned long int x = (320 * b);
  OCR1B = (uint16_t)(x / maxpwmvalue);
}
bool FFans::FanRunning0() {
  unsigned long nowtime = millis();
  //this is only relevent if its been running for weeks
  if (nowtime < globaltime2) {
    globaltime2 = nowtime;
  }
  //This checks to see if the fan has been triggering the interupt
  unsigned long elapsedtime = nowtime - globaltime2;
  if (elapsedtime >= minwaitmils) {
    globaltime2 = nowtime;
    if (tachcount2 - previoustachcount2 == 0) {
      fanrunningflag0 = false;
    } else {
      tachcount2 = previoustachcount2;
      fanrunningflag0 = true;
    }
  }
  return fanrunningflag0;
}
bool FFans::FanRunning1() {
  unsigned long nowtime = millis();
  //this is only relevent if its been running for weeks
  if (nowtime < globaltime3) {
    globaltime3 = nowtime;
  }
  //This checks to see if the fan has been triggering the interupt
  unsigned long elapsedtime = nowtime - globaltime3;
  if (elapsedtime >= minwaitmils) {
    globaltime3 = nowtime;
    if (tachcount3 - previoustachcount3 == 0) {
      fanrunningflag1 = false;
    } else {
      tachcount3 = previoustachcount3;
      fanrunningflag1 = true;
    }
  }
  return fanrunningflag1;
}