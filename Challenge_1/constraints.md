# Challenge 1

Using trimpots to turn servos to a given orrientation.

## Inputs:
- Current state of game
- Two trimpots (one for each servo)
- Accelerometer data for final position

## Outputs:
- single serial value for completed

## Logic:
Master STM will recieve a state from the GUI telling it that we are in this challenge state. The board will enable the relevant functions/interrupts and check for the accelerometer on the IMU to be in the correct position. As the trimpots turn, the servos will turn accordinly and if the accelerometer is in the right orrientation, the completeion signal will TX to the GUI.


