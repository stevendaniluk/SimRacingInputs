#include <Arduino.h>
#include "Encoder.h"

Encoder::Encoder(uint8_t pin_1, uint8_t pin_2, void (*pin_1_isr)(void),
            void (*pin_2_isr)(void))
  : pin_1_(pin_1)
  , pin_2_(pin_2)
  , prev_state_set_(false)
  , prev_delta_(0) {
  pinMode(pin_1_, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_1_), pin_1_isr, CHANGE);

  pinMode(pin_2_, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_2_), pin_2_isr, CHANGE);
}

Encoder::Encoder(uint8_t pin_1, uint8_t pin_2)
  : pin_1_(pin_1)
  , pin_2_(pin_2)
  , prev_state_set_(false)
  , prev_delta_(0) {
  pinMode(pin_1_, INPUT_PULLUP);
  pinMode(pin_2_, INPUT_PULLUP);
}

void Encoder::update() {
  uint8_t state = getEncoderState();
  if (!prev_state_set_) {
    prev_state_ = state;
    prev_state_set_ = true;
    return;
  }

  // We want to send engagements each time the encoder changes state,
  // and disengagements when it is not changing
  int delta = deltaInput(prev_state_, state);

  if (delta != 0) {
    // Got a new input
    uint8_t button = (delta > 0) ? 1 : 0;
    send(button, true);
  }else if (delta != prev_delta_) {
    // No change, but last update there was an input so disengage
    uint8_t button = (prev_delta_ > 0) ? 1 : 0;
    send(button, false);
  }

  prev_delta_ = delta;
  prev_state_ = state;
}

int Encoder::deltaInput(uint8_t prev_state, uint8_t state) {
  if (prev_state == state) {
    return 0;
  }else if (prev_state == 3 && state == 0) {
    // Wrap around increasing
    return 1;
  }else if (prev_state_ == 0 && state == 3) {
    // Wrap around decreasing
    return -1;
  }else {
    return uint8_t(state > prev_state) - uint8_t(state < prev_state);
  }
}

uint8_t Encoder::getEncoderState() {
  bool p1 = digitalRead(pin_1_);
  bool p2 = digitalRead(pin_2_);

  uint8_t state;
  if (!p1 && !p2) {
    state = 0;
  }else if (p1 && !p2) {
    state = 1;
  }else if (p1 && p2) {
    state = 2;
  }else {
    state = 3;
  }

  return state;
}
