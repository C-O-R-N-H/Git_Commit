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
the correct position the user must hold teh board at 0,0 for 2 seconds. if the loacation is correct, the master board will light up and send the code 
for one lock complete. The process must be completed three times to finish teh challenge.

The secondary board will read the accelrometer value and calculate the velocity. It will send that value to the GUI which will send it back to the master board.
If the current position is correct and velocity is 0 for 2 seconds, the main board sends the completion signal. Once all three locks are done, the challenge 
completion code is sent to the GUI
