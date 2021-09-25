// Uncomment the line below to enable printing outputs to serial for every button event
//#define SIM_SERIAL_DEBUG

#include <Joystick.h>
#include "ButtonJoystick.h"
#include "EncoderJoystick.h"
#include "FunkySwitch.h"

// Example steering wheel input for a sim racing rig.
//
// This example attempts to utilize all the available input types with all the possible
// ways of using them. You can add or remove inputs based on your needs to turn this into
// a button box, or anything else. // But be sure to update the pin Id's to monitor as well
// as the total number of inputs below!
//
// This monitors a set of buttons/switches/encoders and issues signals that are recognized
// as joystick inputs by a computer.
//
// This is setup for the following inputs:
//   - 4 Momentary buttons on individual pins
//   - 2 Momentary buttons connected to the same analog input
//   - 1 Encoder
//   - 1 Funky switch (up, down, left, right, push, encoder)

// Funky switch pin Id's
#define PIN_ENC_UP 0
#define PIN_ENC_DOWN 1
#define PIN_UP 2
#define PIN_DOWN 3
#define PIN_LEFT 4
#define PIN_RIGHT 5
#define PIN_PUSH 6

// Momentary buttons pin Id's
#define PIN_B1 7
#define PIN_B2 8
#define PIN_B3 9
#define PIN_B4 10

// Two momentary buttons on one analog input. We define which analog pin, as well as
// the thresholds to classify an input as LOW or HIGH. Anything lower than 120 will
// be LOW, anything higher than 130 will be HIGH.
#define PIN_COMBO A0
#define COMBO_A_LOW 120
#define COMBO_A_HIGH 130

// Encoder pin Id's
#define PIN_ENC_1 11
#define PIN_ENC_2 12

// Here we define the total number of buttons we are monitoring, which is used by
// the joystick library. We also define how many inputs are momentary buttons so
// we know how many to create down below.
const uint8_t TOTAL_BUTTONS = 15;
const uint8_t NUM_BUTTON_INPUTS = 10;

// Create the joystick to send inputs to the computer
Joystick_* joystick;
// An array of all the momentary buttons we'll monitor
ButtonInput* buttons[NUM_BUTTON_INPUTS];
// Our funky switch input
FunkySwitch* funky_switch;
// Out encoder input
EncoderJoystick* encoder;

// Pre define some callbacks for encoder changes
void encUpISR();
void encDownISR();

void setup() {
#ifdef SIM_SERIAL_DEBUG
  Serial.begin(9600);
#endif

  // Setup our joystick input, pretty much all the inputs are false since we are sending
  // everything in as buttons
  joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
    TOTAL_BUTTONS, 0,      // Button Count, Hat Switch Count
    false, false, false,   // X and Y, but no Z Axis
    false, false, false,   // No Rx, Ry, or Rz
    false, false,          // No rudder or throttle
    false, false, false);  // No accelerator, brake, or steering

#ifndef SIM_SERIAL_DEBUG
  joystick->begin();
#endif

  // Create each of our button inputs. We provide it the joystick object to send
  // updates to, an ID (which is what the computer will see), an which pin to monitor
  buttons[0] = new ButtonJoystick(joystick, 0, PIN_B1);
  buttons[1] = new ButtonJoystick(joystick, 1, PIN_B2);
  buttons[2] = new ButtonJoystick(joystick, 2, PIN_B3);
  // By default a low input is considered as the button being depressed, but we could
  // reverse that if desired by adding an extra argument at the end
  buttons[3] = new ButtonJoystick(joystick, 3, PIN_B4, false);
  // For this version we have two buttons connected to a single analog input, with one of
  // the buttons running through a resistor so we see different input voltages. So we also
  // provide the LOW/HIGH input thresholds defined above.
  buttons[8] = new ButtonJoystick(joystick, 4, PIN_COMBO, COMBO_A_LOW, COMBO_A_HIGH);

  // Funky switch. We define an array of button ID's (what the computer sees), the pin ID's
  // to monitor, as well as the callback functions for when we receive encoder updates.
  uint8_t funky_ids [7] = {6, 7, 8, 9, 10, 11, 12};
  uint8_t funky_pins [7] = {PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT, PIN_PUSH, PIN_ENC_UP, PIN_ENC_DOWN};
  funky_switch = new FunkySwitch(joystick, funky_ids, funky_pins, encUpISR, encDownISR);

  // The encoder is just like having two buttons, we provide two ID's and two pins to monitor
  encoder = new EncoderJoystick(joystick, 13, 14, PIN_ENC_1, PIN_ENC_2);
}

void loop() {
  // Here we simply update the funky switch toggle button, update the funky switch encoder,
  // update the other encoder, roll through all our momentary buttons, then sleep until the
  // next cycle
  funky_switch->updateToggle();
  funky_switch->updateEncoder();
  encoder->update();
  for (size_t i = 0; i < NUM_BUTTON_INPUTS; ++i) {
    buttons[i]->update();
  }

  delay(50);
}

// These callbacks need to be defined in the main .ino file. We simply invoke the update method
// on the FunkySwitch object
void encUpISR() {
  funky_switch->encUpChange();
}
void encDownISR() {
  funky_switch->encDownChange();
}
