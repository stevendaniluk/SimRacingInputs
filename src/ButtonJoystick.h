#pragma once

#include <Joystick.h>
#include "ButtonInput.h"

// ButtonJoystick
//
// Reads a pins state and sends a joystick input each time the pin goes HIGH/LOW.
//
// This will only send a single input for each change, it will not
// repeatedly send the input.
class ButtonJoystick : public ButtonInput {
public:
  // Constructor
  //
  // @param joystick: Joystick to issue inputs through
  // @param key: Button Id to issue
  // @param pin: Pin number to read
  // @param low_on: When true, a LOW state will trigger a keystroke
  ButtonJoystick(Joystick_* joystick, uint8_t key, uint8_t pin, bool low_on = true);

  // Constructor
  //
  // @param joystick: Joystick to issue inputs through
  // @param key: Button Id to issue
  // @param pin: Pin number to read
  // @param range_low: Lower bound of analog input range
  // @param range_high: Upper bound of analog input range
  ButtonJoystick(Joystick_* joystick, uint8_t key, uint8_t pin, int range_low, int range_high);

  void send(bool pressed) const override;

private:
  Joystick_* joystick_;
  uint8_t key_;
};
