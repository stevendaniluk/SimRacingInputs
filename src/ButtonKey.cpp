#include <Arduino.h>
#include "ButtonKey.h"

ButtonKey::ButtonKey(char key, uint8_t pin, bool low_on = true)
  : ButtonInput::ButtonInput(pin, low_on)
  , key_(key) {}

void ButtonKey::send(bool pressed) const {
if (pressed) {
#ifdef SIM_SERIAL_DEBUG
  Serial.print("Pressed ");
  Serial.println(key_);
#else
  Keyboard.write(key_);
#endif
}
}
