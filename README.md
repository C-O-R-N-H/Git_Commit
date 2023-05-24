# Git_Commit &#x00A0; :house_with_garden:

Repository for Thursday 9am 'Git_Commit' Major Project MTRX2700

Group Members:
- Richard Gan
- Will Pennell
- Hiroshi Mukhopadhyay
- Quinn Hannam
- Ruchir Bais 

Roles:
- Hiroshi: LIDAR (Challenge 1)
- Quinn: Touch Sensing Controller (Challenge 2)
- Richard: Combination Lock (Challenge 3)
- Ruchir: ADC (Encryption + Pressure Plate)
- Will: GUI and Integration

## Modules
- [LOCK 1 LIDAR](#lock-1--lidar--see_no_evil)
- [LOCK 2 TSC](#lock-2--touch-sensing-controller-tsc--musical_keyboard)
- [LOCK 3 Combination Lock](#lock-3--combination-lock--lock)
- [Encryption ADC](#encryption-adc--lock_with_ink_pen)
- [Pressure Plate ADC](#pressure-plate-adc--bomb)
- [GUI](#gui--desktop_computer)
- [Communication Protocols](#communication-protocols--telephone_receiver)
- [Integration](#integration--poop)
- [PIN-OUT](#pin-out--pushpin)


### LOCK 1 &#x2013; LIDAR &#x00A0; :see_no_evil:
#### Summary
'Lock 1' emulates a laser tripwire security system by utilizing the lidar on the PTU (Pan-Tilt Unit). The objective of this challenge is to cut the beam from the lidar sensor for a given period of time at the specified distance. 
#### High Level Flow Chart
A high level overview of this challenge is illustrated below.


![Tron Major Flow Chart](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/ed56335c-d149-4d5c-8c2c-43f4ae2140a2)
#### Usage
The system can be initiated by pressing the pushbutton on the STM32F3 discovery board, which functions as the "deactivation" mechanism. Once the button is activated, the user must position themselves within the designated distance of the lidar to "cut the wire." In order to successfully deactivate the system, the user must maintain button pressure for a minimum of 3 seconds while within the lidar's range. Upon meeting these conditions, an LED interface will appear, with the LEDs sequentially iterating through one by one. At this point, the challenge is considered complete.



To achieve this functionality, the code incorporates a button interrupt in `button_interrupt.c` to detect when the button is pressed. Once the button press is detected, the interrupt is enabled. Following that, Timer 3 is initialized to track the duration of the button press. During this countdown, the code also utilizes the `check_condition()` function in `timer.c` to verify if the user is within the range of the lidar. Once all the conditions are met, an 'unlocked' flag is set to enable the LEDs to iterate through their sequence, indicating successful completion of the challenge.


#### Testing Procedures
Testing was performed to ensure that the lidar distance being read is accurate and able to be used without any disturbances. As the PTU in use uses Pulse Width Modulation (PWM) for the lidar, arduino serial plotter was used to plot the lidar readings generated from the PWM. This generated a graph that was able to identify that the data being read was noisy and cannot be used without any adjustments. In order to combat this, a moving average was implemented with a window size of 100 as demonstrated in `filter.c`. The readings were then plotted again in arduino serial plotter and it was evident that the moving average had made an impact on the data as it was now able to be used.
Note: PUTTY or any serial reading app can be used instead of arduino to monitor the noise. 

The code implementation is designed to handle both PWM and I2C readings by evaluating the absolute difference between the lidar readings and the target distance. This flexibility is crucial because PTUs typically operate with either PWM or I2C as their preferred method for obtaining lidar values. By considering both PWM and I2C readings, the code ensures that any PTU can be used regardless of the method it employs. The code checks if the absolute difference between the PWM reading and the target distance, as well as the absolute difference between the I2C reading and the target distance, falls within the specified threshold. This adaptable approach accommodates different PTUs and their varying lidar reading mechanisms. The `check_condition` function in the `timer.c` source file effectively demonstrates this versatility.

The fundamental elements of the system are comprised of the button, timer, and lidar reading functionality. To verify the proper functioning of the button interrupt, the `set_led` function in `led.c` was invoked, providing a visual indication of whether the button interrupt is triggered upon pressing the button. Similarly, the timer interrupt was tested using a similar approach. As the timer interrupt is only triggered when the conditions specified in `check_condition` are met (i.e., the button is pressed while the user is within the specified distance range), the `chase_led` function is called, resulting in a visual representation of an LED "chase" effect on the discovery board. Additionally, the conditions can be verified using a serial communication application such as PuTTY or Arduino by printing the values to observe their behavior.

Once all the conditions are successfully satisfied and fully met (i.e., the button is pressed while the user is within the specified distance range for a duration of 3 seconds), a boolean variable named `flashed` is set to true. Subsequently, the `completion_led` function is invoked to indicate the completion of the challenge. At this point, the challenge is considered successfully completed!



### LOCK 2 &#x2013; Touch Sensing Controller (TSC) &#x00A0; :musical_keyboard:
#### Summary
The objective of this challenge is to emulate a high-tech lock, in which you must play a song using hidden items on the shelf. It uses touch capacitance to detect when the hidden objects are pressed. 
#### User Instructions
This section outlines how the user can interface with the TSC module of this project and how to set it up. The software component for this module is contained in the Challenge3 folder.
User inputs: 
 -	6 touch capacitive electrodes 

To interface with the system, the user simply needs to connect an electrode (anything that can conduct electricity) to its respective channel input pin on the discovery board. These pins are listed in the PIN-OUT section of this document. Once all electrodes are connected, a string will be sent through serial when an electrode is tapped by the user. This string corresponds to the electrode that was tapped, and can be used to trigger an event elsewhere, depending on which electrode is tapped. In our case, this event is playing a note corresponding to the tapped electrode. The output notes are as follows:

 -	Channel 1 -> E
 -	Channel 2 -> D#
 -	Channel 3 -> B
 -	Channel 4 -> D
 -	Channel 5 -> C
 -	Channel 6 -> A

It is important to understand that the note playing is not handled by the TSC software. All this component does is send a string with the corresponding channel number in it when an electrode is tapped.  
The electrodes should be connected as follows:
![tsc_crkt (1)](https://github.com/C-O-R-N-H/Git_Commit/assets/126728164/235b70b4-df86-4cac-9841-cbe9acc46247)
The above figure shows the general circuit layout for an individual TSC group, as they are all connected the same way. The only difference between these groups is the GPIO pins they are connected to on the discovery board, detailed in the PIN-OUT section of the document. The electrode parasitic capacitance represents the capacitance of the electrode itself, which changes when the user places their finger on it. This change is what the TSC sensors detect.
To complete our challenge specifically, the user must tap the electrodes (play-doh ducks in this case) in the correct order, playing a song (“Fur Elise” by Beethoven). The order is 1-2-1-2-1-3-4-5-6. Once the notes are played in this order, a special completion string is sent through serial, telling the master board that the challenge is complete, and green LEDs are lit for each correct note played. If an incorrect note is played, the user must restart playing the song from the beginning, and red LEDs are lit. 
This module also implements some error checking. When a max count error is detected due to a channel being faulty, all relevant variables are reset, and sampling capacitors are discharged, effectively resetting the entire module until the error is fixed.
If the user wishes to change the electrodes, they may encounter a minor issue, where the output string is sent twice if the electrode is held for some time. Fixing this issue is detailed in the “Testing procedures” section of this module. 

#### Testing Procedures
This section outlines how the user can test the functionality of the module and what to do in case of some common errors. 

Testing:

If the circuit is correctly wired and all electrodes are firmly connected, the first step to testing this module is checking that tapping an electrode outputs the correct channel number. This can be done by directly checking the serial output of the module. If the user wishes to test this without the serial module, they can set a certain number of LEDs corresponding to the TSC output channel, using the set_led function. 
If this does not work, the user can do the following:

If the user wishes to test the raw sensor readings, they can use sprintf to add the group_sense_values to the output string, and read the data through a serial plotter. The plot should be a very noisy line, which should decrease when the electrode is pressed. 

To test the rolling average value, the user can do the same as above but for the group_roll_av_buffer values. Plotting the raw data and rolling average on the same plot should show how the rolling average values are much more stable than the noisy sensor data, making it much more useable. The rolling average line should be centered within the raw data line. 

Once the rolling average is working, the next thing the user can test is the output channel, without limiting it to a single output. Plotting this should give a flat line when nothing is pressed, and a square pulse when it is pressed. This is accomplished by detecting when the drop happens, and setting the output channel when it occurs. 

If all the above functions correctly, but the output is still incorrect, see the following section for common errors.

Common errors:

-	Everything works fine, except that there are occasionally two strings sent when an electrode is pressed. To fix this issue, increase the size of the CHANNEL_BUFFER value. This increases the number of counts for which a group needs to output 0 before it can be output again, thus decreasing the sensitivity of the sensors. 

-	I have switched my electrodes and now the outputs no longer work correctly. The current code has been tested with a coin separated from touch by tape, and play-doh that is fully exposed. If you are using a metal plate (or something similar), consider covering with a dielectric (something that does not conduct electricity). If not, changing the sensitivity of the rolling average detection or the number of measurements used to detect a drop (ROLL_AVERAGE_BUFFER) could fix the issue.

-	I have everything set up correctly but am not receiving any outputs, or my outputs are very wrong. Double check that all the connections are firmly connected, especially the ground cable. If all your connections are correct and solid, sometimes restarting the board and power cycling can fix this issue. Also ensure that you are initializing all your variables to 0 when starting the program. 

### LOCK 3 &#x2013; Combination Lock &#x00A0; :lock:

#### Summary
You've made it past Stewart's extensive security system. The only thing between you and not failing MTRX2700 is his safe. Use the secondary STM board &#x2013; the controller &#x2013; to move the PTU into the three correct positions to unlock the safe.

#### High Level Flow Chart

![348861480_161077800186860_3664009888897910189_n](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/0b895828-3689-493c-81ee-e3d67349bade)

#### Usage
The yaw of the PTU is controlled by the roll of the secondary STM, and the pitch of the PTU is controlled by the pitch of the STM. The controller should be held so that the micro USB connection is at the end furthest from the user. In this configuration, tilting the controller to the left causes the PTU to turn to the left in the yaw axis. Conversely, tilting the controller to the right makes the PTU swivel to the right. Similarly, increasing the pitch of the controller (tilting the end of the controller furthest from the user up) causes the LIDAR unit on the PTU to tilt up, and vice versa. Whenever the controller is held (roughly) horizontal, the LEDs on the same board will be off. When the controller is tilted enough so that it is not considered horizontal, one LED will light up indicating the direction in which the board is tilted.

Once the position of the PTU matches the desired position, the blue and orange LEDs on the master board will light up. To 'record' the position of the PTU, the controller must be held horizontal (so that the PTU is still) for 2.5 seconds. If the position is correct, the green LEDs on the master board will flash once and the user will progress to the next position. If the position is not correct, the master board will flash red twice and the lock will be reset &#x2013; the user must start from the first position again. Once all three positions have been correctly located in succession, the combination lock is unlocked and the challenge is complete.

#### Valid input
The only input to the challenge is the orientation of the controller &#x2013; specifically, the pitch and roll. The pitch and roll are translated into the desired velocity of the PTU in both the horizontal and vertical axes, which are sent through the GUI to the master board. The values of pitch and roll are both valid in the range -72.1&#x00B0; to 72.1&#x00B0;. If either of the pitch or roll exit the valid range, the new value is ignored and the controller uses the last valid orientation to calculate the PTU velocity.

#### Testing Procedures
All data validation of the controller is handled on the secondary board according to the valid inputs mentioned in the above section. Thus the velocity data that is sent to the master board is always correct. This is tested by turning the controller upside down &#x2013; the PTU continues turning at the last valid velocity before the controller was inverted. When the controller is returned to its upright position and the orientation data is valid again, the PTU resumes moving according to this orientation.

Testing was also performed to ensure that the accelerometer data was sufficiently stable by shaking the controller and tilting it rapidly in different directions. Despite these agitations, the velocity data is always robust, and this is corroborated by the smooth motion of the PTU. Furthermore, the effectiveness of our sliding window mean filter was verified by noting that the motion of the PTU became significantly smoother when the filter was applied.

When the challenge is started, the controller undergoes a callibration phase where it finds and corrects for any zero errors, guaranteeing that the pitch and roll &#x2013; and hence velocity &#x2013; in the starting position is always zero. To test this callibration, we held the controller at various non-horizontal positions during the callibration phase. The data was printed to terminal, and in every case, the orientation and velocity at the starting position was zero. Furthermore, the controller functioned exactly as intended, with the starting position being considered as the stationary position.

The logic of the challenge is tested through various methods. First, we checked that at every stage (position 1, position 2, and position 3), getting the position wrong causes the stage to be reset to position 1. This is confirmed by printing the current stage to the serial port. Conversely, we checked that at every stage, getting the position right causes the user to progress to the next stage and, importantly, if that stage was the last one, the challenge is completed, indicated by all the LEDs on the master board turning on. 

Second, we tested the 'position checking' mechanism of the system. That is, if at any point the PTU is held stationary for 2.5 seconds, the program evaluates whether the position is correct. This can be seen by the fact that whenever the controller is held horizontal for 2.5 seconds, the LEDs on the master board flash green or red, depending on whether the position was correct. On the other hand, as long as the controller is not held horizontal for a full 2.5 seconds, the LEDs on the master board never flash green or red. This confirms that the 'position checking' only occurs when the PTU is stationary for 2.5 seconds, as required.




### Encryption (ADC) &#x00A0; :lock_with_ink_pen:
#### Summary
The STM board uses 2 ADC Channels to stabilize the PTU motion and encryption. The objective of the task is to turn 2 trimpots to a specific orientation to allow the player to “stabilize”, or essentially get rid of the encryption and login.

#### Usage
The login username and password undergo constant encryption with a dynamic encryption key that continually changes. This encryption process is visually represented by a PTU (Pan-Tilt Unit) using a periodic pitch and yaw function. The PTU's movement speed reflects the rate of change of the encryption key. The 2 trimpots act as dial locks, independently affecting the motion of each axis. As the trimpots approach their desired configuration, the PTU's motion gradually diminishes, reducing both the amplitude and rate of change of the encryption key. Eventually, when trimpots align with the required position, the PTU comes to a complete stop, and the encryption key becomes all zeros. This signifies that the encryption is disabled, allowing the user to enter the username and password displayed on the screen, thereby enabling a successful login.


### Pressure Plate (ADC) &#x00A0; :bomb:
#### Summary
The STM board ADC to check for weight kept on the scale. It uses a load cell as a pressure plate, and the objective is to replace the statue/key with another object quick enough to not trigger the alarm.

#### Usage
The load cell is bolted on to 2 wooden plates to create a pressure plate, and it is wired to an op-amp circuit that outputs a load dependent voltage to the board. When the load is removed from the plate, a single LED lights up to show that no load is being detected. After the set specified seconds pass, if the load hasn’t been detected on the plate again, it triggers the alarm. If replaced within the time, the LED turns off and no alarm is triggered. The player must remove and replace the load within the specified seconds to win the escape room.


#### High Level Flow Chart
![WhatsApp Image 2023-05-24 at 8 54 09 PM](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/3e82c4aa-7ccc-46e5-b3b5-01ebd6dc7cbd)

#### Circuit Diagram
![WhatsApp Image 2023-05-24 at 6 24 19 PM](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/127497cd-c027-46ed-a12f-ecfa477d7658)

#### ADC Testing
For the initial testing each ADC channel was connected to the output of a trimpot. Dividing each ADC channel into 8 levels and mapping them to the LEDs allowed us to verify each PIN and channel. 

Once this basic verification is done, multiple ADC values measured by the board can be output via serial communication using the sprintf function to be inspected. Along with this, the set_led function can allow uniquely lighting the LEDS to visually represent the values. Both were used to verify the functionality of the Encryption ADC module, which required 24 levels from 2 separate channels. 

For the Pressure Plate ADC module, the sensitivity of the plate had to be calibrated by adding or reducing the variable resistor value. The desired weight was put on the scale and the resistance was calibrated manually. When no weight is detected, one LED lights up, and when time has passed with the load missing from the plate, all the LEDs light up. This helps us verify if the board is detecting the removal of the load and not raising a flag when the load is replaced within the time limit. 

For troubleshooting and setting ADC circuits, a multimeter is highly recommended, as we need to be able to verify the specific voltage values at different points in the circuit, and not just HIGH or LOW.

Common errors:

-	ADC level is flickering between two consecutive levels: Noise and other inconsistencies may cause an ADC value on the edge of a defined level to flicker to the adjacent level. Implementing a rolling average will reduce such errors. Selecting the resolution of the levels also will impact this. Too many levels will increase the chances of the user setting the ADC near another level, and the narrow ranges will allow the noise to impact the levels more.
-	ADC values are disordered/wrong: Always check which ADC has called the ConvCpltCallBack function and has a new reading available. Understand the voltage input range of the ADC channel and transform your circuit’s output voltage accordingly. Use a multimeter to check wiring and perform general troubleshooting.



### GUI &#x00A0; :desktop_computer:
#### Summary
A Graphical User Interface (GUI) was made to run all the challenges in conjunction from the 'mainframe'.

#### Testing Instructions
To set up and run the GUI for the challenges, ensure that all four boards are connected to the device. Configure the ports used in the `settings.py` file, specifically setting the `ENABLESERIAL` variable to 1. In the `main.py` file, verify that the locks for each challenge are set to 0, indicating that they have not been completed yet. Once these configurations are in place, execute the `main.py` file, which will open the loading screen.

After the loading screen appears, the user can press the "Begin" button to start and attempt any desired challenge. The challenges (1-3) must be completed within the allocated time limit of 5 minutes. The current game state, indicating the current challenge, will be displayed on the LED interface of the board. For example, challenge 1 will have one LED illuminated, representing its active state.

As each challenge is successfully completed, it will be marked as grey on the GUI, indicating its completion. Once all the challenges are completed, the game enters an encrypted state, as discussed in the encryption section mentioned earlier. At this stage, the user can enter their username and password. It is important to note that any incorrect attempts at the password will result in a failed attempt, and the user will have a total of 5 attempts to enter the correct password.

Once the correct username and password are entered, and the challenge is successfully completed, the entire challenge is considered finished!



## Communication Protocols &#x00A0; :telephone_receiver:

### Format
All communication is routed through the GUI as a master controller, ultimately determining the state and status of each board. All communication follows the form; Sentinels -> Mode -> Data -> End of Sequence Character. Each set of communication occurs in a string to ease the reading of data from both the board and GUI.
 <br />
### Modes
Sentinels -> `AA55` <br />
End of Sequence -> `!` <br />
Mode = `1` -> Change state <br />
- Data -> 2 bytes (number for state) <br />

Mode = `2` -> Challenge completed <br />
- Data -> 1 byte (`1` for completed)<br />

Mode = `3` -> Notes being played <br />
- Data -> 6 bytes (1 for each note) <br />

Mode = `4` -> Board to board TX <br />
- Data -> 4 bytes (2 for pitch and yaw respectivly)<br /> 

Mode = `5` -> LIDAR data <br />
- Data -> 4 bytes for distance<br />

Mode = `6` -> LIDAR target <br />
- Data -> 4 bytes for distance<br />

Mode = `7` -> GYRO data <br />
- Data -> 12 bytes (4 for each direction)<br />

### Integration &#x00A0; :poop:

#### Summmary
The Integration of all tasks such that the communication between the GUI and each board is seamless and efficient.

#### Implementation
The implementation of integration begins with the GUI. For every change of frame (state), the GUI sends a state change message to each board. Each board will then respond accordingly to enter a waiting loop for the respective state. In some cases, this may involve the challenge itself for a particular board. If, at any point, the user presses the back button, the boards will all update their state accordingly and send a termination character to close the RX thread in the GUI. Some challenges involve the boards transmitting large amounts of data to the GUI to either display or send to another board, in this case, the message type check within the GUI RX function will handle the message appropriately. If at any point there is a data collision through the transmitting or receiving of data, the string will be discarded so as to not run any unwanted actions. 

For the challenge:<br />
Waiting state: all RX from boards are disabled, hence no action can change anything.<br />
Lock 1: Master board RX is enabled for LIDAR data and end-of-challenge data.<br />
Lock 2: Touch board RX enabled for sound data and end-of-challenge data.<br />
Lock 3: Accel board and Master board RX enabled for board-to-board TX (velocity data) and end-of-challenge data.<br />
Login: If all challenges are complete, ADC board and Master board RX are enabled for board-to-board TX (velocity data) and gyro data for encryption. If not, all challenges are done, all RX is disabled.<br />
Winning: Pres board RX enabled for failed data.<br />

#### Testing Procedures
To verify the functionality of each module after the integration of serial communication begin by testing each board with a serial communicator. Send each respective board the string to begin the challenge associated with it and then waited for the appropriate response (i.e. a string for each duck press from the touch board). After this, connect all the boards to the master through the GUI and run the `main.py` file. Connect all the boards following the below diagram:

![Wiring_Diagram drawio](https://github.com/C-O-R-N-H/Git_Commit/assets/107867775/d501b7fe-422d-4b5f-bb4a-d8e98dd75a08)

As you press through each game state, you should see the LED patterns updating for each challenge. Likewise, each challenge should send the appropriate data back to the GUI. If the data is being transmitted and received correctly, the GUI should also respond accordingly (i.e., if a challenge is completed, the GUI should update to the completion screen).After this, as previously discussed, each challenge should operate as designed and the whole system should run according to the specification defined above. For details regarding the Pin-out or FAQ, see below.





## PIN-OUT &#x00A0; :pushpin:
### Master Board:
USB -> USART1 <br />
PA15 -> PWM1<br />
PA1-> PWM2<br />
PB7 -> SDA<br />
PB6 -> SCL<br />
PA8 -> LASPWM<br />

### ADC Board
USB -> USART1<br />
PA0 -> ADC1 (1)<br />
PA1 -> ADC1 (2)<br />
PA2 -> ADC1 (3)<br />

### Touch Board
USB -> USART1<br />
PA2 -> Group 1 Channel<br />
PA1 -> Group 1 Sample<br />
PA4 -> Group 2 Channel<br />
PA5 -> Group 2 Sample<br />
PB1 -> Group 3 Channel<br />
PB0 -> Group 3 Sample<br />
PA9 -> Group 4 Channel<br />
PA10 -> Group 4 Sample<br />
PB6 -> Group 5 Channel<br />
PB4 -> Group 5 Sample<br />
PB11 -> Group 6 Channel<br />
PB12 -> Group 6 Sample<br />

### Controller Board
USB -> USART1<br />


## FAQS

#### Q: The Lidar value on the GUI doesn't seem to be changing why is that?
#### A: Ensure that the wires are all plugged in correctly as demonstrated by the pin-out. Note: Loose wires are common and often hard to debug.

