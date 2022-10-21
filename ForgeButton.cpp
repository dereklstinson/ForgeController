#include "Arduino.h"
#include "ForgeButton.h"

ForgeButton::ForgeButton(int pin) {
  pinMode(pin, INPUT);
  _pin = pin;
  _delay = 50;
  _lstate = LOW;
}
bool ForgeButton::Pressed() {
  int reading = digitalRead(_pin);

  if (reading != _lstate) {
    _lastdebounce = millis();
  }
  if ((millis() - _lastdebounce) > _delay) {
    if (reading != _state) {
      _state = reading;
      if (_state == HIGH) {
        _lstate = reading;
        return true;
      }
    }
  }
  _lstate = reading;
  return false;
}