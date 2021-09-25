#pragma once

#ifndef SIM_SERIAL_DEBUG
#include <Keyboard.h>
#endif

#include "ButtonInput.h"

// ButtonKey
//
// Reads a pins state and sends a keystroke each time the pin goes HIGH/LOW.
//
// This will only send a single keystroke for each change, it will not
// repeatedly send the keystroke.
class ButtonKey : public ButtonInput {
public:
  // Constructor
  //
  // @param key: Character to send
  // @param pin: Pin number to read
  // @param low_on: When true, a LOW state will trigger a keystroke
  ButtonKey(char key, uint8_t pin, bool low_on = true);

  void send(bool pressed) const override;

private:
  char key_;
};
