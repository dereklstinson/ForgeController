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
void FFans::Begin(){
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
}
unsigned long FFans::CalcRPM0() {
  unsigned long difference = tachcurrent2 - tachprevious2;
if(difference==0){
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
  if(difference==0){
return 0;  
}    
  //two rises per rotation
  //60 seconds in a minute
  //1,000,000 microseconds in a second
  //60000000ms/minute * 1 rotation/(2*difference(microseconds))
  //30000000/difference
  return 30000000 / difference;
}
void FFans::SetPWM1(long int b) {
  long int x = (320 * b);
  long int y = (1000);
  OCR1A = (uint16_t)(x / y);
}
void FFans::SetPWM0(long int b) {
  long int x = (320 * b);
  long int y = (1000);
  OCR1B = (uint16_t)(x / y);
}
bool FFans::FanRunning0() {
  unsigned long nowtime = millis();
  //this is only relevent if its been running for weeks
  if (nowtime < globaltime2) {
    globaltime2 = nowtime;
    tachcount2 = 0;
  }
  //This checks to see if the fan has been triggering the interupt
  unsigned long elapsedtime = nowtime - globaltime2;
  unsigned long rpm = 1;
  if (elapsedtime >= 5000) {
    rpm = tachcount2 / elapsedtime;
    tachcount2 = 0;
    globaltime2 = nowtime;
  }
  if (rpm > 0) {
    return true;
  }
  return false;
}
bool FFans::FanRunning1() {
  unsigned long nowtime = millis();
  //this is only relevent if its been running for weeks
  if (nowtime < globaltime3) {
    globaltime3 = nowtime;
    tachcount3 = 0;
  }
  //This checks to see if the fan has been triggering the interupt
  unsigned long elapsedtime = nowtime - globaltime3;
  unsigned long rpm = 1;
  if (elapsedtime >= 5000) {
    rpm = tachcount3 / elapsedtime;
    tachcount3 = 0;
    globaltime3 = nowtime;
  }
  if (rpm > 0) {
    return true;
  }
  return false;
}