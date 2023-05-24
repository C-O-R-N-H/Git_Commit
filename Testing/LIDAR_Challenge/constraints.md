# Challenge 2

The beam from the LIDAR sensor must be cut for a given period of time at the right distance.

## Inputs:
- Current state of game
- LIDAR value
- Button on master board

## Outputs:
- constant serial data for the LIDAR sensor
- ending completion number through serial

## Logic:
Master STM will recieve a state from the GUI telling it that we are in this challenge state. The board will enable the relevant functions/interrupts. 
The board will wait till the button is pressed at wich point will check if the beam was cut at the right spot. If the beam is cut at the right spot and the button is being held down, the LEDS will begin to light up and once full will send the completion message
