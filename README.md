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
- Ruchir: ADC (Encryption+Pressure Plate)
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
#### User Instructions
Lock 1 emulates a laser tripwire security system by utilizing the lidar on the PTU (Pan-Tilt Unit). The system can be initiated by pressing the pushbutton on the STM32F3 discovery board, which functions as the "deactivation" mechanism. Once the button is activated, the user must position themselves within the designated distance of the lidar to "cut the wire." In order to successfully deactivate the system, the user must maintain button pressure for a minimum of 3 seconds while within the lidar's range. Upon meeting these conditions, an LED interface will appear, with the LEDs sequentially iterating through one by one. At this point, the challenge is considered complete.



To achieve this functionality, the code incorporates a button interrupt in `button_interrupt.c` to detect when the button is pressed. Once the button press is detected, the interrupt is enabled. Following that, Timer 3 is initialized to track the duration of the button press. During this countdown, the code also utilizes the `check_condition()` function in `timer.c` to verify if the user is within the range of the lidar. Once all the conditions are met, an 'unlocked' flag is set to enable the LEDs to iterate through their sequence, indicating successful completion of the challenge.


#### Testing Procedures
Testing was performed to ensure that the lidar distance being read is accurate and able to be used without any disturbances. As the PTU in use uses Pulse Width Modulation (PWM) for the lidar, arduino serial plotter was used to plot the lidar readings generated from the PWM. This generated a graph that was able to identify that the data being read was noisy and cannot be used without any adjustments. In order to combat this, a moving average was implemented with a window size of 100 as demonstrated in `filter.c`. The readings were then plotted again in arduino serial plotter and it was evident that the moving average had made an impact on the data as it was now able to be used.
Note: PUTTY or any serial reading app can be used instead of arduino to monitor the noise. 

The code implementation is designed to handle both PWM and I2C readings by evaluating the absolute difference between the lidar readings and the target distance. This flexibility is crucial because PTUs typically operate with either PWM or I2C as their preferred method for obtaining lidar values. By considering both PWM and I2C readings, the code ensures that any PTU can be used regardless of the method it employs. The code checks if the absolute difference between the PWM reading and the target distance, as well as the absolute difference between the I2C reading and the target distance, falls within the specified threshold. This adaptable approach accommodates different PTUs and their varying lidar reading mechanisms. The `check_condition` function in the `timer.c` source file effectively demonstrates this versatility.

The fundamental elements of the system are comprised of the button, timer, and lidar reading functionality. To verify the proper functioning of the button interrupt, the `set_led` function in `led.c` was invoked, providing a visual indication of whether the button interrupt is triggered upon pressing the button. Similarly, the timer interrupt was tested using a similar approach. As the timer interrupt is only triggered when the conditions specified in `check_condition` are met (i.e., the button is pressed while the user is within the specified distance range), the `chase_led` function is called, resulting in a visual representation of an LED "chase" effect on the discovery board. Additionally, the conditions can be verified using a serial communication application such as PuTTY or Arduino by printing the values to observe their behavior.

Once all the conditions are successfully satisfied and fully met (i.e., the button is pressed while the user is within the specified distance range for a duration of 3 seconds), a boolean variable named `flashed` is set to true. Subsequently, the `completion_led` function is invoked to indicate the completion of the challenge. At this point, the challenge is considered successfully completed!



### LOCK 2 &#x2013; Touch Sensing Controller (TSC) &#x00A0; :musical_keyboard:

#### User Instructions
![tsc_crkt (1)](https://github.com/C-O-R-N-H/Git_Commit/assets/126728164/235b70b4-df86-4cac-9841-cbe9acc46247)
#### Testing Procedures

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

#### Testing




### Pressure Plate (ADC) &#x00A0; :bomb:
#### Summary
The STM board ADC to check for weight kept on the scale. It uses a load cell as a pressure plate, and the objective is to replace the statue/key with another object quick enough to not trigger the alarm.

#### Usage
The load cell is bolted on to 2 wooden plates to create a pressure plate, and it is wired to an op-amp circuit that outputs a load dependent voltage to the board. When the load is removed from the plate, a single LED lights up to show that no load is being detected. After the set specified seconds pass, if the load hasn’t been detected on the plate again, it triggers the alarm. If replaced within the time, the LED turns off and no alarm is triggered. The player must remove and replace the load within the specified seconds to win the escape room.


#### High Level Flow Chart
![WhatsApp Image 2023-05-24 at 8 54 09 PM](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/3e82c4aa-7ccc-46e5-b3b5-01ebd6dc7cbd)

#### Circuit Diagram
![WhatsApp Image 2023-05-24 at 6 24 19 PM](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/127497cd-c027-46ed-a12f-ecfa477d7658)

### GUI &#x00A0; :desktop_computer:
#### Summary
A Graphical User Interface (GUI) was made to run all the challenges in conjunction from the 'mainframe'.

#### Testing Instructions
To set up and run the GUI for the challenges, ensure that all four boards are connected to the device. Configure the ports used in the settings file, specifically setting the serial port to 1. In the `main.py` file, verify that the locks for each challenge are set to 0, indicating that they have not been completed yet. Once these configurations are in place, execute the `main.py` file, which will open the loading screen.

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

#### User Instructions

#### Testing Procedures




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
#### A: Ensure that the wires are all plugged in correctly as demonstrated by the pin-out.

