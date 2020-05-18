# DIY Load Cell Sim Racing Pedals
This repository contains simple Arduino scripts for interfacing with load cells, and presenting to a Windows PC as a joystick. Currently this reads two load cells, and sends both brake and throttle inputs through the Joystick library.

## Dependencies
* The HX711 Arduino Library to interface with the load cells
* Matthew Heironimus' ArudinoJoystickLibrary, to send the values to the desktop as a joystick device. See https://github.com/MHeironimus/ArduinoJoystickLibrary

## Hardware Requirements
* The Joystick library will only work on Arduino devices that are based on the ATMega32u4. The scripts here have been tested with an Arduino Leonardo.
* Two HX711 Load Cell Amplifiers
* Two straight bar load cells

## A note on read speeds
By default the HX711 reads at 10Hz, which is a tad slow for throttle and brake inputs. The Sparkfun version of the HX711 Load Cell Amplifier can be switched to 80Hz by cutting the trace between the two "RATE" pads on the back of the board, using a hobbyist knife. While this does increase the noise, the noise is still small compared to the values read from the load cell.

