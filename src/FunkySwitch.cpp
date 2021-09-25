#include <Arduino.h>
#include "FunkySwitch.h"

FunkySwitch::FunkySwitch(Joystick_* joystick, uint8_t ids [7], uint8_t pins [7],
            void (*enc_ip_isr)(void), void (*enc_down_isr)(void))
  : joystick_(joystick)
  , up_(joystick, ids[IDS::UP], pins[IDS::UP])
  , down_(joystick, ids[IDS::DOWN], pins[IDS::DOWN])
  , left_(joystick, ids[IDS::LEFT], pins[IDS::LEFT])
  , right_(joystick, ids[IDS::RIGHT], pins[IDS::RIGHT])
  , push_(joystick, ids[IDS::PUSH], pins[IDS::PUSH])
  , enc_up_engaged_(false)
  , enc_down_engaged_(false)
  , new_enc_input_(false)
  , prev_enc_state_(false)
  {

    for (size_t i = 0; i < 7; ++i) {
      ids_[i] = ids[i];
      pins_[i] = pins[i];
    }

    pinMode(pins_[IDS::ENC_UP], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pins_[IDS::ENC_UP]), enc_ip_isr, CHANGE);

    pinMode(pins_[IDS::ENC_DOWN], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pins_[IDS::ENC_DOWN]), enc_down_isr, CHANGE);
}

void FunkySwitch::updateToggle() const {
  bool something_pressed = false;
  something_pressed |= up_.update();
  something_pressed |= down_.update();
  something_pressed |= left_.update();
  something_pressed |= right_.update();

  if (!something_pressed) {
    push_.update();
  }
}

void FunkySwitch::updateEncoder() {
  // Release any engaged buttons
  if (enc_up_engaged_) {
    sendEncoderInput(IDS::ENC_UP, false);
    enc_up_engaged_ = false;
    return;
  }
  if (enc_down_engaged_) {
    sendEncoderInput(IDS::ENC_DOWN, false);
    enc_down_engaged_ = false;
    return;
  }

  // Handle any new inputs
  if (new_enc_input_) {
    sendEncoderInput(new_enc_input_id_, true);
    if (new_enc_input_id_ == IDS::ENC_UP) {
      enc_up_engaged_ = true;
    }else if (new_enc_input_id_ == IDS::ENC_DOWN) {
      enc_down_engaged_ = true;
    }

    new_enc_input_ = false;
  }
}

void FunkySwitch::sendEncoderInput(uint8_t id, bool input) {
#ifdef SIM_SERIAL_DEBUG
    if (input) {
      Serial.print("Pressing ");
    }else {
      Serial.print("Releasing ");
    }

    Serial.println(ids_[id]);
#else
    joystick_->setButton(ids_[id], input);
#endif
}

void FunkySwitch::processChange(uint8_t id_index) {
  bool up_state = digitalRead(pins_[IDS::ENC_UP]);
  bool down_state = digitalRead(pins_[IDS::ENC_DOWN]);

  // A new encoder position is reached when both pins are the same.
  //
  // We also need to check that both pins have changed because with
  // this encoder it occasionally (and annoyingly) happens that a pin
  // can bounce high and low before changing.
  if (up_state == down_state && up_state != prev_enc_state_) {
    // Record the input and save the state
    new_enc_input_ = true;
    new_enc_input_id_ = id_index;
    prev_enc_state_ = up_state;
  }
}
