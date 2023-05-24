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
- [LIDAR](#lidar)
- [TSC](#tsc)
- [Combination Lock](#combination-lock)
- [GUI](#gui)

### ADC

### LIDAR

#### User Instructions

#### Testing Procedures
![Tron Major Flow Chart](https://github.com/C-O-R-N-H/Git_Commit/assets/126120093/ed56335c-d149-4d5c-8c2c-43f4ae2140a2)



### TSC

#### User Instructions

#### Testing Procedures

### Combination Lock

#### User Instructions

#### Testing Procedures

#### High Level Flow Chart
```mermaid
graph TD
Start((Start)) --> IsBoardStill{Is the board horizontal?}

IsBoardStill -- Yes --> IsBoardStill

IsBoardStill -- No --> MovePTU[Move PTU]
MovePTU --> IsBoardStill

IsBoardStill -- Board is held horizontal for 2.5 seconds --> IsPosCorrect{Is the position correct?}
IsPosCorrect -- No --> ResetLock[Reset the lock]
ResetLock --> IsBoardStill

IsPosCorrect -- Yes --> IsLastPos{Was that the last position?}
IsLastPos -- No --> NextPos[Move on to the next position]
NextPos --> IsBoardStill

IsLastPos -- Yes --> ChallengeComplete((Challenge complete))
```


### GUI



## Communication Protocols:

### Format
Sentinels -> Mode -> Data -> End of Sequence <br />
### Modes
Sentinels -> `AA55` <br />
End of Sequence -> `!` <br />
Mode = `1` -> Change state <br />
- Data -> 2 bytes (number for state) <br />

Mode = `2` -> Challenge completed <br />
- Data -> <br />

Mode = `3` -> Notes being played <br />
- Data -> 6 bytes (1 for each note) <br />

Mode = `4` -> Board to board TX <br />
- Data -> <br /> 4 bytes 

Mode = `5` -> LIDAR data <br />
- Data -> 4 bytes for distance<br />

Mode = `6` -> LIDAR target <br />
- Data -> 4 bytes for distance<br />

Mode = `7` -> GYRO data <br />
- Data -> <br />

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
