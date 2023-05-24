# Git_Commit

Repository for Thursday 9am 'Git_Commit' Major Project MTRX2700

Group Members:
- Richard Gan
- Will Pennell
- Hiroshi Mukhopadhyay
- Quinn Hannam
- Ruchir Bais 

Roles:
- LIDAR (Challenge 1) Hiroshi
- TSC (Challenge 2) Quinn
- Combination Lock (Challenge 3) Richard
- ADC (Encryption)
- GUI Will

## Modules
- [LOCK 1 LIDAR](#lock-1-lidar)
- [LOCK 2 TSC](#lock-2-tsc)
- [LOCK 3 Combination Lock](#lock-3-combination-lock)
- [ENCRYPTION ADC](#encryption-adc)
- [GUI](#gui)
- [Integration](#integration)

### ADC

### LOCK 1 (Lidar)
Lock 1 emulates a laser tripwire security system by utilizing the lidar on the PTU (Pan-Tilt Unit). The system can be initiated by pressing the pushbutton on the STM32F3 discovery board, which functions as the "deactivation" mechanism. Once the button is activated, the user must position themselves within the designated distance of the lidar to "cut the wire." In order to successfully deactivate the system, the user must maintain button pressure for a minimum of 3 seconds while within the lidar's range. Upon meeting these conditions, an LED interface will appear, with the LEDs sequentially iterating through one by one. At this point, the challenge is considered complete.

A high level overview of this challenge is illustrated below.


![Tron Major Flow Chart](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/ed56335c-d149-4d5c-8c2c-43f4ae2140a2)

To achieve this functionality, the code incorporates a button interrupt in `button_interrupt.c` to detect when the button is pressed. Once the button press is detected, the interrupt is enabled. Following that, Timer 3 is initialized to track the duration of the button press. During this countdown, the code also utilizes the `check_condition()` function in `timer.c` to verify if the user is within the range of the lidar. Once all the conditions are met, an 'unlocked' flag is set to enable the LEDs to iterate through their sequence, indicating successful completion of the challenge.

#### User Instructions

#### Testing Procedures



### LOCK 2 (TSC)

#### User Instructions

#### Testing Procedures

### LOCK 3 (Combination Lock)

#### User Instructions

#### Testing Procedures

#### High Level Flow Chart

![348861480_161077800186860_3664009888897910189_n](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/0b895828-3689-493c-81ee-e3d67349bade)

### ENCRYPTION (ADC)
The login username and password undergo constant encryption with a dynamic encryption key that continually changes. This encryption process is visually represented by a PTU (Pan-Tilt Unit) using a periodic pitch and yaw function. The PTU's movement speed reflects the rate of change of the encryption key. The 2 trimpots act as dial locks, independently affecting the motion of each axis. As the trimpots approach their desired configuration, the PTU's motion gradually diminishes, reducing both the amplitude and rate of change of the encryption key. Eventually, when trimpots align with the required position, the PTU comes to a complete stop, and the encryption key becomes all zeros. This signifies that the encryption is disabled, allowing the user to enter the username and password displayed on the screen, thereby enabling a successful login.


### GUI



## Communication Protocols:

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




## PIN-OUT:
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
