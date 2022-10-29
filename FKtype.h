/*
Forge Menu temperature sensor
*/
#ifndef FKtype_h
#define FKtype_h

#include "Arduino.h"
#include <SPI.h>
#include "Adafruit_MAX31855.h"
class FKtype {
  public:
    FKtype();
    
    bool Begin();
  double  ReadC();

  
  private:
   Adafruit_MAX31855 thermalk=Adafruit_MAX31855(6, 7, 5);
   
};
#endif