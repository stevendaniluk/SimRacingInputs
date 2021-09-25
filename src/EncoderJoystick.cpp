#include <Arduino.h>
#include "EncoderJoystick.h"

EncoderJoystick::EncoderJoystick(Joystick_* joystick, uint8_t key_inc, uint8_t key_dec,
                uint8_t pin_1, uint8_t pin_2, void (*pin_1_isr)(void),
                void (*pin_2_isr)(void))
  : Encoder::Encoder(pin_1, pin_2, pin_1_isr, pin_2_isr)
  , joystick_(joystick)
  , key_inc_(key_inc)
  , key_dec_(key_dec){}

EncoderJoystick::EncoderJoystick(Joystick_* joystick, uint8_t key_inc, uint8_t key_dec,
              uint8_t pin_1, uint8_t pin_2)
: Encoder::Encoder(pin_1, pin_2)
, joystick_(joystick)
, key_inc_(key_inc)
, key_dec_(key_dec){}

void EncoderJoystick::send(uint8_t button, bool val) const {
  uint8_t key = button > 0 ? key_inc_ : key_dec_;
#ifdef SIM_SERIAL_DEBUG
  if (val) {
    Serial.print("Pressed ");
  }else {
    Serial.print("Released ");
  }

  Serial.println(key);
#else
  joystick_->setButton(key, val);
#endif
}
