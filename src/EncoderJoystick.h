#pragma once

#include <Joystick.h>
#include "Encoder.h"

// EncoderJoystick
//
// Sends a single joystick input each time the encoder changes.
class EncoderJoystick : public Encoder {
public:
  // Constructor that uses interrupts
  //
  // @param joystick: Joystick to issue inputs through
  // @param key_inc button Id for increases
  // @param key_dec button Id for decreases
  // @param pin_1: First pin to monitor
  // @param pin_2: Second pin to monitor
  // @param pin_1_isr: Interrupt function for changes to pin 1
  // @param pin_2_isr: Interrupt function for changes to pin 2
  EncoderJoystick(Joystick_* joystick, uint8_t key_inc, uint8_t key_dec,
                  uint8_t pin_1, uint8_t pin_2, void (*pin_1_isr)(void),
                  void (*pin_2_isr)(void));

  // Constructor that relies on calls to update()
  //
  // @param joystick: Joystick to issue inputs through
  // @param key_inc button Id for increases
  // @param key_dec button Id for decreases
  // @param pin_1: First pin to monitor
  // @param pin_2: Second pin to monitor
  EncoderJoystick(Joystick_* joystick, uint8_t key_inc, uint8_t key_dec,
                  uint8_t pin_1, uint8_t pin_2);

  void send(uint8_t button, bool val) const override;

private:
  Joystick_* joystick_;
  uint8_t key_inc_;
  uint8_t key_dec_;
};
