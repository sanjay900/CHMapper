SanPIE
======
A linux programmable input emulator

SanPIE is an input emulator that supports using lua as an intermediate language to script connections between different input and output devices.
Currently, SanPIE supports as inputs:
-  Wii Remotes + extensions
-  Controllers
-  Serial
-  MIDI
-  MIDI over Serial
-  Keyboard

And as outputs, it supports:
-  Virtual Controller
-  Keyboard
-  MIDI
-  Serial

Requirements:
- evdev (libevdev-dev on ubuntu)
- rtmidi 
- system modules enabled for uinput and evdev (modprobe uinput and modprobe evdev)