# midipedals

Midi Pedals is a project that creates a midi-usb interface with 4 pedals.


Connected between pins D0, D1, D2, D3 and GND of teensy 2.0 board.

A short press on D0, D1, D2, D3 will trigger channel 8, C3, C#3, D3, D#3

A long press (>500ms) on the same pedals will trigger E3, F3, F#3, G3 (but
also the short-press notes).



