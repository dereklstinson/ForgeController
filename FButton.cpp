#include "Arduino.h"
#include "FButton.h"

FButton::FButton(int pin) {
  pinMode(pin, INPUT);
  _pin = pin;
  _delay = 50;
  _lstate = LOW;
  _holddelay = 500;
}
bool FButton::Pressed() {
  int reading = digitalRead(_pin);

  if (reading != _lstate) {
    _holddelay=500;
    _lastdebounce = millis();
  }
  if ((millis() - _lastdebounce) > _delay) {
    if (reading != _state) {
      _state = reading;
      if (_state == HIGH) {
        _lstate = reading;
        return true;
      }
    } else if (_state == HIGH) {
      if (millis() - _lastdebounce > _holddelay) {
        _lstate = reading;
        return true;
      }
    }
  }
  _lstate = reading;
  return false;
}