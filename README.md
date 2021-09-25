# SimRacingInputs

Simple Arduino library to aid in creating custom input devices for sim racing, which can appear either as a keyboard or joystick input.

This library provides functionality for:
* Momentary buttons
* Encoders
* Funky switches (5 way toggle + encoder)

Some inputs can be configured as keyboard or joystick inputs. However all inputs must be of the same type, you cannot mix keyboard and joystick inputs on the same device!

Check out [SimRacingProjects](https://github.com/stevendaniluk/SimRacingProjects) for some examples of this library used in real projects.

## Installation

### Dependencies
You will also need to install the following libraries:
* [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary)  
* [Keyboard](https://www.arduino.cc/reference/en/language/functions/usb/keyboard/)

### Setup
1. Download the latest release zip file from the [Releases page](https://github.com/stevendaniluk/SimRacingInputs/releases)
1. Add the downloaded library to the Arduino IDE `Sketch --> Include Library --> Add .ZIP Library`

You will now be able to see the example sketches under `File --> Examples`, and can include inputs in your own sketch (e.g. `#include "FunkySwitch.h"`).

## Examples

- `ButtonBoxJoystick` - Simple button box with 4 momentary push buttons recognized as joystick inputs
- `ButtonBoxKeyboard` - Simple button box with 4 momentary push buttons recognized as keyboard inputs
- `Wheel` - A more complicated arrangement with multiple momentary buttons, an encoder, and a funky switch

## Usage

### Multiple Microcontrollers
When multiple microcontrollers of the same type are detected by Windows they will have separate device Id's, but each of them will always have the same vendor Id and product name. Some games will not be able to distinguish between the devices.

To get around this, you can temporarily modify the board info used by Arduino to have a different vendor Id and product name, then reflash the microcontroller. Note, you shouldn't really be doing this because these Id's are supposed to be reserved by vendors, but I am not aware of another way to resolve this issue.

There is a `boards.txt` file for all the boards the Arduino IDE is aware of. For macOS this will either be in of the following directories (or some variation of it for different board manufacturers):
```bash
/Applications/Arduino.app/Contents/Java/hardware/arduino/avr
/Users/$USER/Library/Arduino15/packages/arduino/hardware/avr
/Users/$USER/Documents/Arduino/hardware/arduino/avr
```

Find your board and change the entires for:
```
board_name.build.usb_product
board_name.build.vid
```

## Classes

**ButtonInput**: Base class for all momentary buttons that defines the interface. Can operate on digital inputs which can be configered to send on LOW or HIGH, or analogue inputs which trigger based on input thresholds.

**ButtonKey**: Momentary button input that sends updates as keyboard button presses.

**ButtonJoystick**: Momentary button input that sends updates as joystick inputs.

**Encoder**: Encoder that utilizes interrupts on two pins to trigger state changes. This can currently only be configured as a joystick input.

**FunkySwitch**: Has 5 momentary button inputs (up, down, left, right, push) and one encoder. This can currently only be configured as a joystick input.
