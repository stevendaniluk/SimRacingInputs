#include <Arduino.h>
#include "ButtonInput.h"

ButtonInput::ButtonInput(uint8_t pin, bool low_on = true)
  : pin_(pin)
  , low_on_(low_on)
  , analog_(false)
  , prev_state_(!low_on) {
  pinMode(pin, INPUT_PULLUP);
}

ButtonInput::ButtonInput(uint8_t pin, int range_low, int range_high)
  : pin_(pin)
  , analog_(true)
  , range_low_(range_low)
  , range_high_(range_high)
  , prev_state_(false) {
  pinMode(pin, INPUT_PULLUP);
}

bool ButtonInput::isPressed() const {
  if (!analog_) {
    if (low_on_) {
      return digitalRead(pin_) == LOW;
    }else {
      return digitalRead(pin_) == HIGH;
    }
  }else {
    int input = analogRead(pin_);
    return input > range_low_ && input < range_high_;
  }
}

bool ButtonInput::update() {
  bool pressed = isPressed();
  if (pressed != prev_state_) {
    send(pressed);
  }

  prev_state_ = pressed;
  return pressed;
}
