import serial

def init():
    #frame data
    global LOGIN_SYSTEM
    
    global current_state
    current_state = 0

    global FAILED_ATTEMPTS
    FAILED_ATTEMPTS = 0

    global LOCK_1_COMPLETE
    LOCK_1_COMPLETE = 1

    global LOCK_2_COMPLETE
    LOCK_2_COMPLETE = 1

    global LOCK_3_COMPLETE
    LOCK_3_COMPLETE = 1

    global PRES_FAIl
    PRES_FAIl = 0

    global LOCK_1_HINT
    LOCK_1_HINT = 0

    global LOCK_2_HINT
    LOCK_2_HINT = 0

    global LOCK_3_HINT
    LOCK_3_HINT = 0

    global LOCK_4_HINT
    LOCK_4_HINT = 0

    global CURRENT_TIME
    CURRENT_TIME = 300

    global TESTING
    TESTING = 1

    #communication
    global ENABLE_SERIAL
    ENABLE_SERIAL = 0

    global SENTINAL
    SENTINAL = "AA55"
    
    global MASTER_SERIAL
    
    global TOUCH_BOARD
    
    global ACCEL_BOARD
    
    global ADC_BOARD

    global PRESSURE_BOARD
    
    if ENABLE_SERIAL == 1:
        MASTER_SERIAL = serial.Serial('/dev/tty.usbmodem11403', 115200)
        TOUCH_BOARD = serial.Serial('/dev/tty.usbmodem11303', 115200)
        ACCEL_BOARD = serial.Serial('/dev/tty.usbmodem11203', 115200)
        ADC_BOARD = serial.Serial('/dev/tty.usbmodem11103', 115200)
        PRESSURE_BOARD = serial.Serial('/dev/tty.usbmodem1203', 115200)
    else:
        MASTER_SERIAL = serial.Serial()
        TOUCH_BOARD = serial.Serial()
        ACCEL_BOARD = serial.Serial()
        ADC_BOARD = serial.Serial()
        PRESSURE_BOARD = serial.Serial()
    

    #Notes
    global E
    E = 'GUI/Duck/E.wav'

    global Eflat
    Eflat = 'GUI/Duck/Eflat.wav'

    global B
    B = 'GUI/Duck/B.wav'

    global D
    D = 'GUI/Duck/D.wav'

    global C
    C = 'GUI/Duck/C.wav'

    global A
    A = 'GUI/Duck/A.wav'

    #Sensor

    global LIDAR_DIST
    LIDAR_DIST = 0

    global GYRO_ARRAY
    GYRO_ARRAY = ""
