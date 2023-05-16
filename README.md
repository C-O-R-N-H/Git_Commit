# Git_Commit

Repository for Thursday 9am 'Git_Commit' Major Project MTRX2700

Group Members:
- Richard Gan
- Will Pennell
- Hiroshi Mukhopadhyay
- Quinn Hannam
- Ruchir Bais 

Roles:
- ADC (Challenge 1) Ruchir
- Lidar (Challenge 2) Hiroshi
- TSC (Challenge 3) Quinn
- Combination Lock (Challenge 4) Richard
- GUI Will

## Modules



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
- Data -> <br />



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
PA2 -> ADC1 (1)<br />
PA3 -> ADC1 (2)<br />

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
