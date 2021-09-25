#pragma once

// Encoder
//
// Monitors two pin states on a continuous encoder. This assumes that
// the order of pin states across positions is as follows:
//   pos 1: low-low
//   pos 2: high-low
//   pos 3: high-high
//   pos 4: low-high
//
// This will only send a single input for each change, it will not
// repeatedly send the input.
class Encoder {
public:
  // Constructor that uses interrupts
  //
  // @param pin_1: First pin to monitor
  // @param pin_2: Second pin to monitor
  // @param pin_1_isr: Interrupt function for changes to pin 1
  // @param pin_2_isr: Interrupt function for changes to pin 2
  Encoder(uint8_t pin_1, uint8_t pin_2, void (*pin_1_isr)(void),
              void (*pin_2_isr)(void));

  // Constructor that relies on calls to update()
  //
  // @param pin_1: First pin to monitor
  // @param pin_2: Second pin to monitor
  Encoder(uint8_t pin_1, uint8_t pin_2);

  void pin1Change() {
    update();
  }

  void pin2Change() {
    update();
  }

  // send
  //
  // Sends the input for this encoder
  //
  // @param button: 0=decreasing, 1=increasing
  // @param val: true=engaged, false=not engaged
  virtual void send(uint8_t button, bool val) const = 0;

  // update
  //
  // Updates the state of the encoder and sends a signal each time the
  // encoder is moved (engagement) or remains the same for two consecutive
  // cycles (disengaged).
  void update();

private:

  // deltaInput
  //
  // @return: Change in encoder position {-1, 0, 1}
  int deltaInput(uint8_t prev_state, uint8_t state);

  // getEncoderState
  //
  // The encoder pins change as follows:
  //   pos 1: low-low
  //   pos 2: high-low
  //   pos 3: high-high
  //   pos 4: low-high
  //
  // @return: Encoder position [0, 3]
  uint8_t getEncoderState();

  uint8_t pin_1_;
  uint8_t pin_2_;
  bool prev_state_set_;
  uint8_t prev_state_;
  int prev_delta_;
};
