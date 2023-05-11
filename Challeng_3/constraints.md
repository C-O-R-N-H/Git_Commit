# Challenge 3

Using touch capacitance to play a song

## Inputs:
- Current state of game
- 6 'buttons'

## Outputs:
- every note pressed returns a value over serial to play the audio for that note
- single serial value for completed

## Logic:
Master STM will recieve a state from the GUI telling it that we are in this challenge state. The board will enable the relevant functions/interrupts.
The master board will send the signal to the board running the Touch capacitance module to activate. The touch board will, everytime a key is pressed we check if it is the correct note for teh sequence overall checking for teh sequence.
after each 'button' is pressed, the touch board will send a value corresponding to ech button over serial to the GUI which will play the note. onec teh challenge is complete, the board will send teh challenge completion value.


