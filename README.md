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
- Ruchir: ADC (Encryption)
- Will: GUI

## Modules
- [LOCK 1 LIDAR](#lock-1-lidar)
- [LOCK 2 TSC](#lock-2-tsc)
- [LOCK 3 Combination Lock](#lock-3-combination-lock)
- [ENCRYPTION ADC](#encryption-adc)
- [GUI](#gui)
- [Integration](#integration)


### LOCK 1 &#x2013; LIDAR &#x00A0; :see_no_evil:
Lock 1 emulates a laser tripwire security system by utilizing the lidar on the PTU (Pan-Tilt Unit). The system can be initiated by pressing the pushbutton on the STM32F3 discovery board, which functions as the "deactivation" mechanism. Once the button is activated, the user must position themselves within the designated distance of the lidar to "cut the wire." In order to successfully deactivate the system, the user must maintain button pressure for a minimum of 3 seconds while within the lidar's range. Upon meeting these conditions, an LED interface will appear, with the LEDs sequentially iterating through one by one. At this point, the challenge is considered complete.

#### High Level Flow Chart
A high level overview of this challenge is illustrated below.


![Tron Major Flow Chart](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/ed56335c-d149-4d5c-8c2c-43f4ae2140a2)

To achieve this functionality, the code incorporates a button interrupt in `button_interrupt.c` to detect when the button is pressed. Once the button press is detected, the interrupt is enabled. Following that, Timer 3 is initialized to track the duration of the button press. During this countdown, the code also utilizes the `check_condition()` function in `timer.c` to verify if the user is within the range of the lidar. Once all the conditions are met, an 'unlocked' flag is set to enable the LEDs to iterate through their sequence, indicating successful completion of the challenge.

#### User Instructions

#### Testing Procedures



### LOCK 2 &#x2013; Touch Sensing Controller (TSC) &#x00A0; :musical_keyboard:

#### User Instructions

#### Testing Procedures

### LOCK 3 &#x2013; Combination Lock &#x00A0; :lock:

#### Summary
In this challenge, a secondary STM board &#x2013; the controller &#x2013; is used to control the movement (pan and tilt) of the PTU. The objective of the task is to move the PTU into three specific positions in order to 'unlock' the combination lock. 

#### Usage
The yaw of the PTU is controlled by the roll of the secondary STM, and the pitch of the PTU is controlled by the pitch of the STM. The controller should be held so that the micro USB connection is at the end furthest from the user. In this configuration, tilting the controller to the left causes the PTU to turn to the left in the yaw axis. Conversely, tilting the controller to the right makes the PTU swivel to the right. Similarly, increasing the pitch of the controller (tilting the end of the controller furthest from the user up) causes the LIDAR unit on the PTU to tilt up, and vice versa. Whenever the controller is held (roughly) horizontal, the LEDs on the same board will be off. When the controller is tilted enough so that it is not considered horizontal, one LED will light up indicating the direction in which the board is tilted.

Once the position of the PTU matches the desired position, the blue and orange LEDs on the master board will light up. To 'record' the position of the PTU, the controller must be held horizontal (so that the PTU is still) for 2.5 seconds. If the position is correct, the green LEDs on the master board will flash once and the user will progress to the next position. If the position is not correct, the master board will flash red twice and the lock will be reset &#x2013; the user must start from the first position again. Once all three positions have been correctly located in succession, the combination lock is unlocked and the challenge is complete.

#### Valid input
The only input to the challenge is the orientation of the controller &#x2013; specifically, the pitch and roll. The pitch and roll are translated into the desired velocity of the PTU in both the horizontal and vertical axes, which are sent through the GUI to the master board. The values of pitch and roll are both valid in the range -72.1&#x00B0; to 72.1&#x00B0;. If either of the pitch or roll exit the valid range, the new value is ignored and the controller uses the last valid orientation to calculate the PTU velocity.

#### Testing
All data validation of the controller is handled on the secondary board according to the valid inputs mentioned in the above section. Thus the velocity data that is sent to the master board is always correct. This is tested by turning the controller upside down &#x2013; the PTU continues turning at the last valid velocity before the controller was inverted. When the controller is returned to its upright position and the orientation data is valid again, the PTU resumes moving according to this orientation.

Testing was also performed to ensure that the accelerometer data was sufficiently stable by shaking the controller and tilting it rapidly in different directions. Despite these agitations, the velocity data is always robust, and this is corroborated by the smooth motion of the PTU. Furthermore, the effectiveness of our sliding window mean filter was verified by noting that the motion of the PTU became significantly smoother when the filter was applied.

When the challenge is started, the controller undergoes a callibration phase where it finds and corrects for any zero errors, guaranteeing that the pitch and roll &#x2013; and hence velocity &#x2013; in the starting position is always zero. To test this callibration, we held the controller at various non-horizontal positions during the callibration phase. The data was printed to terminal, and in every case, the orientation and velocity at the starting position was zero. Furthermore, the controller functioned exactly as intended, with the starting position being considered as the stationary position.

The logic of the challenge is tested through various methods. First, we checked that at every stage (position 1, position 2, and position 3), getting the position wrong causes the stage to be reset to position 1. This is confirmed by printing the current stage to the serial port. Conversely, we checked that at every stage, getting the position right causes the user to progress to the next stage and, importantly, if that stage was the last one, the challenge is completed, indicated by all the LEDs on the master board turning on. 

Second, we tested the 'position checking' mechanism of the system. That is, if at any point the PTU is held stationary for 2.5 seconds, the program evaluates whether the position is correct. This can be seen by the fact that whenever the controller is held horizontal for 2.5 seconds, the LEDs on the master board flash green or red, depending on whether the position was correct. On the other hand, as long as the controller is not held horizontal for a full 2.5 seconds, the LEDs on the master board never flash green or red. This confirms that the 'position checking' only occurs when the PTU is stationary for 2.5 seconds, as required.

#### High Level Flow Chart

![348861480_161077800186860_3664009888897910189_n](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/0b895828-3689-493c-81ee-e3d67349bade)



### Encryption (ADC) &#x00A0; :lock_with_ink_pen:
The login username and password undergo constant encryption with a dynamic encryption key that continually changes. This encryption process is visually represented by a PTU (Pan-Tilt Unit) using a periodic pitch and yaw function. The PTU's movement speed reflects the rate of change of the encryption key. The 2 trimpots act as dial locks, independently affecting the motion of each axis. As the trimpots approach their desired configuration, the PTU's motion gradually diminishes, reducing both the amplitude and rate of change of the encryption key. Eventually, when trimpots align with the required position, the PTU comes to a complete stop, and the encryption key becomes all zeros. This signifies that the encryption is disabled, allowing the user to enter the username and password displayed on the screen, thereby enabling a successful login.





### Load Cell (ADC) &#x00A0; :bomb:
![WhatsApp Image 2023-05-24 at 6 24 19 PM](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/127497cd-c027-46ed-a12f-ecfa477d7658)

### GUI &#x00A0; :desktop_computer:



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

### Integration

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
PA1 -> Group 1 Sample<br />
PA2 -> Group 1 Channel<br />
PB0 -> Group 3 Sample<br />
PB1 -> Group 3 Channel<br />
PB11 -> Group 6 Channel<br />
PB12 -> Group 6 Sample<br />
PA4 -> Group 2 Channel<br />
PA5 -> Group 2 Sample<br />
PA9 -> Group 4 Channel<br />
PA10 -> Group 4 Sample<br />
PB4 -> Group 5 Sample<br />
PB6 -> Group 5 Channel<br />
