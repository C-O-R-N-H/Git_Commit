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
- GUI Will

## Modules
- [ADC](#adc)
- [LOCK 1 LIDAR](#lock-1-lidar)
- [LOCK 2 TSC](#lock-2-tsc)
- [LOCK 3 Combination Lock](#lock-3-combination-lock)
- [GUI](#gui)
- [Integration](#integration)

### ADC

### LOCK 1 (Lidar)
Lock 1 attempts to mimic a laser tripwire security system through use of the lidar on the PTU. The system works by pressing the pushbutton on the STM32F3 discovery board as this 'deactivates' the system. When the button is pressed, the user must now 'cut the wire' by being

#### User Instructions

#### Testing Procedures
![Tron Major Flow Chart](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/ed56335c-d149-4d5c-8c2c-43f4ae2140a2)



### LOCK 2 (TSC)

#### User Instructions

#### Testing Procedures

### LOCK 3 (Combination Lock)

#### User Instructions

#### Testing Procedures

#### High Level Flow Chart

![348861480_161077800186860_3664009888897910189_n](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/0b895828-3689-493c-81ee-e3d67349bade)



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
