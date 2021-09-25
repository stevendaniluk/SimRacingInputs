#pragma once

#include <Joystick.h>
#include "ButtonJoystick.h"

// FunkySwitch
//
// Represents a "Funky Switch" input which has a joystick with left,
// right, up, down, and press inputs as well as an encoder. This
// will monitor the pin states and send button presses for each input.
//
// This will only send a single button press for each change, it will not
// repeatedly send the input.
//
// This uses interrupts for the encoder inputs. The interrupt handlers must
// be defined outside this class and the function pointers are provided as
// inputs.
class FunkySwitch {
public:

  // Array indices for button ids and pins
  enum IDS {UP = 0, DOWN, LEFT, RIGHT, PUSH, ENC_UP, ENC_DOWN};

  // Constructor
  //
  // @param joystick: Joystick to issue inputs through
  // @param ids: Array of button ids to press in the order:
  //             [up, down, left, right, push, enc +, enc -]
  // @param pins: Array of pins to monitor for changes in the order:
  //             [up, down, left, right, push, enc +, enc -]
  // @param enc_up_isr: Interrupt function for changes to the enc + pin
  // @param enc_down_isr: Interrupt function for changes to the enc - pin
  FunkySwitch(Joystick_* joystick, uint8_t ids [7], uint8_t pins [7],
              void (*enc_ip_isr)(void), void (*enc_down_isr)(void));

  // updateToggle
  //
  // Updates the states for all the toggle inputs (up, down, left, right, push)
  // and sends their inputs.
  void updateToggle() const;

  // updateEncoder
  //
  // Sends the encoder inputs when the state has changed
  void updateEncoder();

  // encUpChange
  //
  // Function to call from the encoder + interrupt for indicating a
  // change in the pin state.
  void encUpChange() {
    processChange(IDS::ENC_UP);
  }

  // encDownChange
  //
  // Function to call from the encoder - interrupt for indicating a
  // change in the pin state.
  void encDownChange() {
    processChange(IDS::ENC_DOWN);
  }

private:

  // sendEncoderInput
  //
  // Sends the encoder input signal
  //
  // @param id: Button id index
  // @param input: Input state (true=engaged)
  void sendEncoderInput(uint8_t id, bool input);

  // processChange
  //
  // Handles process encoder pin changes. This will look at both pins for
  // when they match up, and which one was triggered first.
  //
  // This has some filtering to handle false positives triggers (which
  // unfortunately seem to happen with the interrupts) and excessively rapid
  // pin changes, which can look like th encoder rapidly going back and forth.
  //
  // @param id_index: Pin id index of the pin to process the change for
  void processChange(uint8_t id_index);

  Joystick_* joystick_;

  ButtonJoystick up_;
  ButtonJoystick down_;
  ButtonJoystick left_;
  ButtonJoystick right_;
  ButtonJoystick push_;

  uint8_t ids_ [7];
  uint8_t pins_ [7];

  bool enc_up_engaged_;
  bool enc_down_engaged_;

  volatile bool new_enc_input_;
  volatile uint8_t new_enc_input_id_;
  volatile bool prev_enc_state_ = false;
};
