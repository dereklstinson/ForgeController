#include "Arduino.h"
#include "FKtype.h"
#include "SPI.h"
FKtype::FKtype() {
}
bool FKtype::Begin() {
  return thermalk.begin();
}
double FKtype::ReadC() {
  return thermalk.readCelsius();
}