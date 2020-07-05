# Orpheus

Orginal library made by squix78 availible [here](https://github.com/squix78/MAX7219LedMatrix). Orpheus is a library for Arduino IDE that can display text on one or multiple MAX7219 8x8 led matrices.

## Installing library in Arduino IDE

- open Arduino IDE
- open preferences and take note of the 'Sketchbook location' path
- navigate into the `libraries` sub folder at that path (e.g. with terminal)
- clone this Git repository into that folder
- restart Arduino IDE
- you should now find the Orpheus library in Sketch > Include Library

## Connecting the module(s) to the ESP8266

|LED Matrix | ESP8266                     |
|-----------|-----------------------------|
|VCC        | +3.3V                       |
|GND        | GND                         |
|DIN        |GPIO13 (HSPID)               |
|CS         |Choose free GPIO, e.g. GPIO2 |
|CLK        |GPIO14 (HSPICLK)             |
