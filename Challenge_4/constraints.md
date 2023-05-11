# Challenge 4

Use a secondary board accelerometer to turn the servos as to solve a combination lock

## Inputs:
- Current state of game
- Accelerometer from secondary board
- current servo position

## Outputs:
- Single serial value for completed
- Serial output for number of completed locks

## Logic:
Master STM will recieve a state from the GUI telling it that we are in this challenge state. The board will enable the relevant functions/interrupts.
The GUI will also send the enable signal to the slave board. As the user moves the slave board it will vary the velocity of each servo. Once the servos
the correct position the user must hold the board at velocity (0, 0) for 2 seconds. if the location is correct, the master board will light up and send the code 
for one lock complete. The process must be completed three times to finish the challenge.

## Implementation:
The secondary board will read the accelerometer value, translate that into pitch and roll values, and calculate the velocity in both directions. It will send that value to the GUI which will send it to the master board.
If the velocity is held at (0, 0) for 2 seconds, the main board will record the orientation of the PTU. If the orientation is correct, the main board sends the completion signal for that combination.
Once all three locks are done, the challenge completion code is sent to the GUI.
