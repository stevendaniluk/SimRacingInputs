// Uncomment the line below to enable printing outputs to serial for every button event
//#define SIM_SERIAL_DEBUG

#include <Joystick.h>
#include "ButtonJoystick.h"

// Example button box input for a sim racing rig. This monitors a set of buttons and
// issues signals that are recognized as joystick inputs by a computer.
//
// This is setup for the following inputs:
//   - 4 Momentary buttons on individual pins
//
// See the Wheel example for more complicated input types.

// Momentary buttons pin Id's
#define PIN_B1 1
#define PIN_B2 2
#define PIN_B3 3
#define PIN_B4 4

// Here we define the total number of buttons we are monitoring, which is used by
// the joystick library and below to create all the necessary buttons
const uint8_t TOTAL_BUTTONS = 4;

// Create the joystick to send inputs to the computer
Joystick_* joystick;
// An array of all the momentary buttons we'll monitor
ButtonInput* buttons[TOTAL_BUTTONS];

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
  buttons[3] = new ButtonJoystick(joystick, 3, PIN_B4);
}

void loop() {
  // Here we simply roll through all our momentary buttons, then sleep until the next cycle
  for (size_t i = 0; i < TOTAL_BUTTONS; ++i) {
    buttons[i]->update();
  }

  delay(50);
}
