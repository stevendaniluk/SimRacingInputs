#pragma once

// ButtonInput
//
// Reads a pins state and sends an input each time the pin goes HIGH/LOW.
// This is an abstract class, the input method needs to be defined
//
// This will only send a single input for each change, it will not
// repeatedly send the input.
class ButtonInput {
public:
  // Constructor
  //
  // Digital input version.
  //
  // @param pin: Pin number to read
  // @param low_on: When true, a LOW state will trigger an input
  ButtonInput(uint8_t pin, bool low_on = true);

  // Constructor
  //
  // Analog input version
  //
  // @param pin: Pin number to read
  // @param range_low: Lower bound of analog input range
  // @param range_high: Upper bound of analog input range
  ButtonInput(uint8_t pin, int range_low, int range_high);

  // isPressed
  //
  // @return: True when the button is pressed
  bool isPressed() const;

  // send
  //
  // Sends the input for this button
  //
  // @param pressed: State of the button
  virtual void send(bool pressed) const = 0;

  // update
  //
  // Checks if the button is pressed and sends the keystroke.
  // This will only send the key stroke on the first call
  // while the buttone is pressed.
  //
  // @return: True when the button is pressed
  bool update();

private:
  uint8_t pin_;
  bool low_on_;
  bool analog_;
  int range_low_;
  int range_high_;
  bool prev_state_;
};
