#include <Arduino.h>
#include "ButtonJoystick.h"

ButtonJoystick::ButtonJoystick(Joystick_* joystick, uint8_t key, uint8_t pin, bool low_on = true)
  : ButtonInput::ButtonInput(pin, low_on)
  , joystick_(joystick)
  , key_(key) {}

ButtonJoystick::ButtonJoystick(Joystick_* joystick, uint8_t key, uint8_t pin, int range_low, int range_high)
: ButtonInput::ButtonInput(pin, range_low, range_high)
, joystick_(joystick)
, key_(key) {}

void ButtonJoystick::send(bool pressed) const {
#ifdef SIM_SERIAL_DEBUG
  Serial.print(key_);
  Serial.print(" state: ");
  Serial.println(pressed);
#else
  joystick_->setButton(key_, pressed);
#endif
}
