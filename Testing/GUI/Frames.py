import customtkinter
from PIL import Image
import os
import random
import string
import settings
import time
import _thread
from playsound import playsound
import serial
import math


def rx_data(reader):
    read_str = ""
    reader.reset_input_buffer()
    while(1):
        value = "!"
        try:
            value = reader.read().decode("utf-8")
        except:
            print(f"error reading from {reader}.")
        
        read_str += (value)
        if (value == '!'): #end of string
            if (read_str.startswith("AA55")): #check sentinal
                mode = read_str[4]
                #print(mode)
                if (int(mode) == 2): #check mode for challenge completed
                    complete = read_str[5]
                    if (int(complete) == 1): #if completed
                        if type(settings.LOGIN_SYSTEM) == Lock_1:
                            settings.LOCK_1_COMPLETE = 1
                        elif type(settings.LOGIN_SYSTEM) == Lock_2:
                            settings.LOCK_2_COMPLETE = 1
                        elif type(settings.LOGIN_SYSTEM) == Lock_3:
                            settings.LOCK_3_COMPLETE = 1
                        break
                if (int(mode) == 3): #notes
                    data = read_str[5:11]
                    if (int(data[0]) >= 1):
                        play_note(settings.E)
                    if (int(data[1]) >= 1):
                        play_note(settings.Eflat)
                    if (int(data[2]) >= 1):
                        play_note(settings.B)
                    if (int(data[3]) >= 1):
                        play_note(settings.D)
                    if (int(data[4]) >= 1):
                        play_note(settings.C)
                    if (int(data[5]) >= 1):
                        play_note(settings.A)
                    print(data)
                if (int(mode) == 5): #lidar
                    data = read_str[5:9]
                    settings.LIDAR_DIST = int(data)
                    print(data)
                if (int(mode) == 4): #board to board
                    #print(read_str)
                    #string = "AA5541212!"
                    settings.MASTER_SERIAL.write(str(read_str).encode())
                if (int(mode) == 0): #teasting
                    print(read_str)
                if (int(mode) == 7): #gyro
                    #print(read_str[5:21])
                    settings.GYRO_ARRAY = ""
                    roll = str(abs(int(read_str[5:10])))
                    pitch = str(abs(int(read_str[10:15])))
                    yaw = str(abs(int(read_str[15:20])))
                    if (int(roll) < 500):
                        roll = "000"
                    if (int(pitch) < 500):
                        pitch = "000"
                    if (int(yaw) < 500):
                        yaw = "000"


                    settings.GYRO_ARRAY += roll
                    settings.GYRO_ARRAY += pitch
                    settings.GYRO_ARRAY += yaw
                    #print(settings.GYRO_ARRAY)
            
            #else:
                #print(read_str)
                #print("error statement")


            if (read_str == "!"): #if back is pressed
                print("back")
                break
            #print(read_str)
            read_str = ""
        #print(read_str)

def play(note):
    playsound(note)
    return         

def play_note(note):
    try:
        _thread.start_new_thread( play , (note,))
    except:
        print ("Error: unable to start thread")

def count_down(root):
    while(1):
        time.sleep(1)
        settings.CURRENT_TIME -= 1
        
class Frames:
    
    def set_state(self, state):
        self.state = state
        self.font_text = "Voice Activated BB"

class Loading_Frame(Frames):
    def __init__(self, root):
        
        super().set_state(0)
        settings.current_state = self.state
        self.root = root
        self.counter = 0

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())

        #bg image
        current_path = os.path.dirname(os.path.realpath(__file__))
        self.bg_image = customtkinter.CTkImage(Image.open(current_path + "/loading.jpg"),size=(900, 600))
        self.bg_image_label = customtkinter.CTkLabel(master = root, image=self.bg_image, text = "")
        self.bg_image_label.grid(row = 0, column = 0)


        self.progressbar = customtkinter.CTkProgressBar(master = self.root, width = 700, height = 40, corner_radius = 2, determinate_speed = 2, progress_color = "DeepSkyBlue3")
        self.progressbar.set(0)
        self.progressbar.grid(row = 0, column = 0, pady = 100)

        self.bg = 'GUI/Notes/bg.wav'

        #play_note(self.bg)
        
    def update(self):
        if self.counter < 24:
            self.progressbar.step()
            self.counter += 1
        else:
            self.progressbar.stop()
            self.story()

    def open(self):
        
        self.progressbar.destroy()
        #start timer
        try:
            _thread.start_new_thread( count_down , (self.root,))
        except:
            print ("Error: unable to start thread")
        settings.LOGIN_SYSTEM = Open_Frame(self.root)
    
    def story(self):
        settings.current_state = 2
        self.progressbar.destroy()
        self.text_frame = customtkinter.CTkFrame(master = self.root)
        self.text_frame.grid(row = 0, column = 0, sticky = "ns")

        self.story = customtkinter.CTkLabel(master = self.text_frame, text = "You have been commissioned to steal \nthe artifact within Prof. Worrall's \nprivate vault. You have gained access \nto the database that controls his \nsecurity system, and must now \nbreak through the locks. Failure \nto complete the challenges before the \ntimer finishes will trigger the \nProfessor's alarm system and immediately \nalert the police to your location. \n\nYou cannot afford to be caught.", font = (self.font_text, 28), text_color = "white")
        self.story.grid(row = 0, column = 0, pady=30, padx=10)

        self.button1 = customtkinter.CTkButton(master=self.text_frame, width=20,height=80, text="Begin", font = (self.font_text,30),hover_color = "dark green", hover = True, command = self.open, fg_color = "DeepSkyBlue4")
        self.button1.grid(row=1, column=0, padx=20, pady=15, sticky="ew")

class Open_Frame(Frames):
    
    def __init__(self, root):
        super().set_state(1)
        settings.current_state = self.state
        self.root = root
        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())
        


        current_path = os.path.dirname(os.path.realpath(__file__))
        self.bg_image = customtkinter.CTkImage(Image.open(current_path + "/bg_image.jpg"),size=(900, 600))
        self.bg_image_label = customtkinter.CTkLabel(master = root, image=self.bg_image, text = "")
        self.bg_image_label.grid(row = 0, column = 0)
        
        #frame
        self.frame_name = customtkinter.CTkFrame(master = root)
        self.frame_name.grid(row = 0, column = 0, sticky = "ns")

        #header
        self.label = customtkinter.CTkLabel(master = self.frame_name, text = "Welcome to the DUC-S",font = (self.font_text, 40))  #defensive underground chamber of stewart
        self.label.grid(row = 0, column = 0, pady=12, padx=50, columnspan = 2)

        self.seconds = settings.CURRENT_TIME%60
        self.minutes = settings.CURRENT_TIME//60

        self.timer = customtkinter.CTkLabel(master = self.frame_name, text = f'{self.minutes:{"0"}{2}} : {self.seconds:{"0"}{2}}',font = (self.font_text, 35), text_color = "red")
        self.timer.grid(row = 1, column = 0, pady=12, padx=100, columnspan = 2)

        #buttons
        self.button1 = customtkinter.CTkButton(master=self.frame_name, width=20,height=80, text="Login", font = (self.font_text,30),hover_color = "dark green", hover = True, command = self.login, fg_color = "DeepSkyBlue4")
        self.button1.grid(row=2, column=0, padx=20, pady=35, sticky="ew")

        self.button2 = customtkinter.CTkButton(master=self.frame_name, width=20,height=80, text="Lock 1", font = (self.font_text,30), command = self.lock_1, fg_color = "DeepSkyBlue4")
        self.button2.grid(row=2, column=1, padx=20, pady=35, sticky="ew")
        self.button3 = customtkinter.CTkButton(master=self.frame_name, width=20,height=80, text="Lock 2", font = (self.font_text,30), command = self.lock_2, fg_color = "DeepSkyBlue4")
        self.button3.grid(row=3, column=0, padx=20, pady=35, sticky="ew")
        
        self.button4 = customtkinter.CTkButton(master=self.frame_name, width=20,height=80, text="Lock 3", font = (self.font_text,30), command = self.lock_3, fg_color = "DeepSkyBlue4")
        self.button4.grid(row=3, column=1, padx=20, pady=35, sticky="ew")
        self.button5 = customtkinter.CTkButton(master=self.frame_name, width=20,height=80, text="Surrender", font = (self.font_text,30), hover_color = "red", hover = True, command = self.end, fg_color = "DeepSkyBlue4")
        self.button5.grid(row=4, column=0, padx=20, pady=35, sticky="ew", columnspan = 2)



        if settings.LOCK_1_COMPLETE == 1:
            self.button2.configure(hover = True, hover_color = "grey", command = None, fg_color = 'grey')
        if settings.LOCK_2_COMPLETE == 1:
            self.button3.configure(hover = True, hover_color = "grey", command = None, fg_color = 'grey')
        if settings.LOCK_3_COMPLETE == 1:
            self.button4.configure(hover = True, hover_color = "grey", command = None, fg_color = 'grey')
        
        if settings.LOCK_1_COMPLETE == 1 and settings.LOCK_2_COMPLETE == 1 and settings.LOCK_3_COMPLETE == 1:
            self.button1.configure(command = self.enc)

        # self.status = "AA55103!"
        # settings.MASTER_SERIAL.write(str(self.state).encode())

    def timer_update(self):
        self.seconds = settings.CURRENT_TIME%60
        self.minutes = settings.CURRENT_TIME//60
        self.timer.configure(text = f'{self.minutes:{"0"}{2}} : {self.seconds:{"0"}{2}}')

    def end(self):
        self.root.destroy()    
    def login(self):
        self.frame_name.destroy()
        settings.LOGIN_SYSTEM = Login_Frame(self.root)
    def lock_1(self):
        self.frame_name.destroy()
        settings.LOGIN_SYSTEM = Lock_1(self.root)
    def lock_2(self):
        self.frame_name.destroy()
        settings.LOGIN_SYSTEM = Lock_2(self.root)
    def lock_3(self):
        self.frame_name.destroy()
        settings.LOGIN_SYSTEM = Lock_3(self.root)
    def enc(self):
        self.frame_name.destroy()
        settings.LOGIN_SYSTEM = ENC(self.root)  

class Login_Frame(Frames):
    
    
    def __init__(self, root):
        super().set_state(2)
        settings.current_state = self.state
        
        if settings.ENABLE_SERIAL == 1:
            if settings.LOCK_1_COMPLETE == 1 and settings.LOCK_2_COMPLETE == 1 and settings.LOCK_3_COMPLETE == 1:
                self.status = settings.SENTINAL + "105!"
                try:
                    _thread.start_new_thread( rx_data , (settings.MASTER_SERIAL, ))
                except:
                    print ("Error: unable to start thread")
                try:
                    _thread.start_new_thread( rx_data , (settings.ADC_BOARD, ))
                except:
                    print ("Error: unable to start thread")
            else:
                self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

            

        

        self.root = root
        self.username_code = self.random_str(6)
        self.password_data = self.random_str(6)
        self.password_cmp = self.password_data
        self.password_code = ""
        #self.password_code = self.password_data[0:2]
        if settings.LOCK_1_COMPLETE == 1:
            self.password_code = self.password_code + self.password_data[0:2]
        else:
            self.password_code = self.password_code + "__"
        if settings.LOCK_2_COMPLETE == 1:
            self.password_code = self.password_code + self.password_data[2:4]
        else:
            self.password_code = self.password_code + "__"
        if settings.LOCK_3_COMPLETE == 1:
            self.password_code = self.password_code + self.password_data[4:6]
        else:
            self.password_code = self.password_code + "__"
        
        self.encryption = random.randint(10000, 999999)

        

        #frame
        self.frame_entry = customtkinter.CTkFrame(master = root)
        self.frame_entry.grid(row = 0, column = 0, sticky = "ns")

        #header
        self.label = customtkinter.CTkLabel(master = self.frame_entry, text = "Login",font = (self.font_text, 50))
        self.label.grid(row = 0, column = 0, pady=30, padx=10)

        self.label = customtkinter.CTkLabel(master = self.frame_entry, text = f"Username: {self.username_code}      Password: {self.password_code}",font = (self.font_text, 25))
        self.label.grid(row = 1, column = 0, pady=10, padx=100)

        if settings.LOCK_1_COMPLETE == 1 and settings.LOCK_2_COMPLETE == 1 and settings.LOCK_3_COMPLETE == 1:
            self.labenc = customtkinter.CTkLabel(master = self.frame_entry, text = f"Encryption: {self.encryption}",font = (self.font_text, 25))
            self.labenc.grid(row = 2, column = 0, pady=10, padx=100)

        self.user = customtkinter.CTkEntry(master = self.frame_entry, placeholder_text="Username", width = 200, height = 50, font = (self.font_text, 25))
        self.user.grid(row = 3, column = 0, pady=12, padx=100)

        self.password = customtkinter.CTkEntry(master = self.frame_entry, placeholder_text="Password", show= "*", width = 200, height = 50, font = (self.font_text, 25))
        self.password.grid(row = 5, column = 0, pady=12, padx=200)

        self.button = customtkinter.CTkButton(master=self.frame_entry, text="Login", command = self.login, width = 200, height = 50, hover = True, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        self.button.grid(row = 7, column = 0, pady=12, padx=200)
        
        self.button = customtkinter.CTkButton(master=self.frame_entry, text="Back", command = self.clear, width = 200, height = 50, hover = True, hover_color = "blue", font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        self.button.grid(row = 9, column = 0, pady=12, padx=200)

        # if settings.LOCK_1_COMPLETE == 0 or settings.LOCK_2_COMPLETE == 0 or settings.LOCK_3_COMPLETE == 0:
        #     self.button = customtkinter.CTkButton(master=self.frame_entry, text="Back", command = self.clear, width = 200, height = 50, hover = True, hover_color = "blue", font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        #     self.button.grid(row = 9, column = 0, pady=12, padx=200)

        #self.label = customtkinter.CTkLabel(master = self.frame_entry, text = f"{ord(self.password_data[0])} ",font = (self.font_text, 18), text_color = "red")
        #self.label.grid(row = 10, column = 0, pady=10, padx=200)

        if settings.FAILED_ATTEMPTS >=1:
            self.label = customtkinter.CTkLabel(master = self.frame_entry, text = f"{settings.FAILED_ATTEMPTS} Failed Attempt",font = (self.font_text, 18), text_color = "red")
            self.label.grid(row = 8, column = 0, pady=10, padx=200)

    def login(self):
        if (self.user.get() == self.username_code and self.password.get() == self.password_cmp):
            self.win()
        else:
            settings.FAILED_ATTEMPTS += 1
            self.label = customtkinter.CTkLabel(master = self.frame_entry, text = f"{settings.FAILED_ATTEMPTS} Failed Attempt",font = (self.font_text, 18), text_color = "red")
            self.label.grid(row = 8, column = 0, pady=10, padx=200)
            if settings.FAILED_ATTEMPTS == 5:
                self.lost()
    def clear(self):
        self.frame_entry.destroy()
        settings.LOGIN_SYSTEM = Open_Frame(self.root) 
    
    def win(self):
        self.frame_entry.destroy()
        settings.LOGIN_SYSTEM = Winning(self.root)
    def lost(self):
        self.frame_entry.destroy()
        settings.LOGIN_SYSTEM = Lost(self.root)
    def random_str(self, length):
        self.letters = string.ascii_lowercase
        self.result_str = ''.join(random.choice(self.letters) for i in range(length))
        return self.result_str
    
    def update_enc(self):
        if settings.LOCK_1_COMPLETE == 1 and settings.LOCK_2_COMPLETE == 1 and settings.LOCK_3_COMPLETE == 1:
            self.labenc.configure(text = f"Encryption: {settings.GYRO_ARRAY}")
            
            if settings.GYRO_ARRAY == "":
                self.password_cmp = "hello"
            elif int(settings.GYRO_ARRAY) == 0:
                self.password_cmp = self.password_data
            else:
                self.password_cmp = "hello"

class Lock_1(Frames):
    #number = random.randint(1,20)
    def __init__(self, root):
        super().set_state(4)
        settings.current_state = self.state
        self.number = random.randrange(110,290, 10)
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "102!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

            settings.MASTER_SERIAL.write(("AA556" + str(self.number*10) + "!").encode())

            try:
                _thread.start_new_thread( rx_data , (settings.MASTER_SERIAL, ))
            except:
                print ("Error: unable to start thread")
            

        #riddle frame
        self.frame_riddle = customtkinter.CTkFrame(master = root)
        self.frame_riddle.grid(row = 0, column = 0, sticky = "ns")

        #text
        self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  f"{self.number}",font = (self.font_text, 30), text_color = "red")
        self.riddle.grid(row = 0, column = 0, pady=10, padx=100)
        self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  "You must cut the beam.",font = (self.font_text, 25), text_color = "light green")
        self.riddle.grid(row = 1, column = 0, pady=10, padx=100)

        #hint
        if settings.LOCK_1_HINT == 0:
            self.button_hint = customtkinter.CTkButton(master=self.frame_riddle, text="Help", width = 200, height = 50, hover = True, hover_color = "purple", command = self.hint, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
            self.button_hint.grid(row = 5, column = 0, pady=12, padx=100)
        else:
            self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  f"Cut the LIDAR beam exactly \n{self.number} cm from the sensor\n and hold the button.",font = (self.font_text, 25), text_color = "light blue")
            self.riddle.grid(row = 5, column = 0, pady=10, padx=50)

        self.button_back = customtkinter.CTkButton(master=self.frame_riddle, text="Back", width = 200, height = 50, hover = True, hover_color = "blue", command = self.back, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        self.button_back.grid(row = 6, column = 0, pady=12, padx=100)

        if settings.TESTING == 1:
            self.complete_button = customtkinter.CTkButton(master=self.frame_riddle, text="Complete", width = 200, height = 50, hover = True, hover_color = "light green", command = self.complete, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
            self.complete_button.grid(row = 7, column = 0, pady=12, padx=200)

        self.lidar_bar = customtkinter.CTkProgressBar(master = self.frame_riddle, orientation="horizontal", height = 20, width = 400)
        self.lidar_bar.grid(row = 8, column = 0, padx = 12, pady = 100)

        
        
    def bar_update(self):
        self.lidar_bar.set((settings.LIDAR_DIST-1000)/2000)
        if self.lidar_bar.get() > ((self.number-100)/200)-0.1 and self.lidar_bar.get() < ((self.number-100)/200)+0.1:
            self.lidar_bar.configure(progress_color = "red")
        else:
            self.lidar_bar.configure(progress_color = "blue")

    def back(self):
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Open_Frame(self.root) 

    def hint(self):
        self.button_hint.destroy()
        settings.CURRENT_TIME -= 60
        settings.LOCK_1_HINT = 1
        self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  f"Cut the LIDAR beam exactly \n{self.number} cm from the sensor\n and hold the button.",font = (self.font_text, 25), text_color = "light blue")
        self.riddle.grid(row = 5, column = 0, pady=10, padx=50)
    def complete(self):
        settings.LOCK_1_COMPLETE = 1
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Completed_Challenge(self.root)

class Lock_2(Frames):
    def __init__(self, root):
        super().set_state(5)
        settings.current_state = self.state
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "103!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

            try:
                _thread.start_new_thread( rx_data , (settings.TOUCH_BOARD, ))
            except:
                print ("Error: unable to start thread")

        #riddle frame
        self.frame_riddle = customtkinter.CTkFrame(master = root)
        self.frame_riddle.grid(row = 0, column = 0, sticky = "ns")

        #text
        self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  "Beethoven loved anything furry \nthat he could name elise.",font = (self.font_text, 25), text_color = "light green")
        self.riddle.grid(row = 0, column = 0, pady=10, padx=60)

        #hint
        if settings.LOCK_2_HINT == 0:
            self.button_hint = customtkinter.CTkButton(master=self.frame_riddle, text="Help", width = 200, height = 50, hover = True, hover_color = "purple", command = self.hint, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
            self.button_hint.grid(row = 5, column = 0, pady=12, padx=100)
        else:
            current_path = os.path.dirname(os.path.realpath(__file__))
            self.hint = customtkinter.CTkImage(Image.open(current_path + "/fur.png"),size=(300, 75))
            self.hint = customtkinter.CTkLabel(master = self.frame_riddle, image=self.hint, text = "")
            self.hint.grid(row = 5, column = 0)

        current_path = os.path.dirname(os.path.realpath(__file__))
        self.alpha = customtkinter.CTkImage(Image.open(current_path + "/alph.png"),size=(500, 200))
        self.alpha = customtkinter.CTkLabel(master = self.frame_riddle, image=self.alpha, text = "")
        self.alpha.grid(row = 6, column = 0, pady = 10)
        
        
        self.button_back = customtkinter.CTkButton(master=self.frame_riddle, text="Back", width = 200, height = 50, hover = True, hover_color = "blue", command = self.back, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        self.button_back.grid(row = 7, column = 0, pady=12, padx=100)

        #notes
        self.E = 'GUI/Duck/E.wav'
        self.Eflat = 'GUI/Duck/Eflat.wav'
        self.B = 'GUI/Duck/B.wav'
        self.D = 'GUI/Duck/D.wav'
        self.C = 'GUI/Duck/C.wav'
        self.A = 'GUI/Duck/A.wav'

        if settings.TESTING == 1:
            self.complete_button = customtkinter.CTkButton(master=self.frame_riddle, text="Complete", width = 200, height = 50, hover = True, hover_color = "light green", command = self.complete, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
            self.complete_button.grid(row = 8, column = 0, pady=12, padx=200)

             #piano tiles
            # self.enote = customtkinter.CTkButton(master=self.frame_riddle, text="E", width = 10, height = 20, hover = True, hover_color = "light green", command = lambda: self.play_note(self.E), font = (self.font_text, 15))
            # self.enote.grid(row = 9, column = 0, pady=5, padx=10)

            # self.eflatnote = customtkinter.CTkButton(master=self.frame_riddle, text="D#", width = 10, height = 20, hover = True, hover_color = "light green", command = lambda: self.play_note(self.Eflat), font = (self.font_text, 15))
            # self.eflatnote.grid(row = 10, column = 0, pady=5, padx=10)

            # self.eflatnote = customtkinter.CTkButton(master=self.frame_riddle, text="B", width = 10, height = 20, hover = True, hover_color = "light green", command = lambda: self.play_note(self.B), font = (self.font_text, 15))
            # self.eflatnote.grid(row = 11, column = 0, pady=5, padx=10)

            # self.eflatnote = customtkinter.CTkButton(master=self.frame_riddle, text="D", width = 10, height = 20, hover = True, hover_color = "light green", command = lambda: self.play_note(self.D), font = (self.font_text, 15))
            # self.eflatnote.grid(row = 12, column = 0, pady=5, padx=10)

            # self.eflatnote = customtkinter.CTkButton(master=self.frame_riddle, text="C", width = 10, height = 20, hover = True, hover_color = "light green", command = lambda: self.play_note(self.C), font = (self.font_text, 15))
            # self.eflatnote.grid(row = 13, column = 0, pady=5, padx=10)

            # self.eflatnote = customtkinter.CTkButton(master=self.frame_riddle, text="A", width = 10, height = 20, hover = True, hover_color = "light green", command = lambda: self.play_note(self.A), font = (self.font_text, 15))
            # self.eflatnote.grid(row = 14, column = 0, pady=5, padx=10)




       

    def play_note(self,note):
        try:
            _thread.start_new_thread( self.play , (note,))
        except:
            print ("Error: unable to start thread")
    
    def play(self, note):
        playsound(note)
        return

    def back(self):
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Open_Frame(self.root) 

    def hint(self):
        self.button_hint.destroy()
        settings.CURRENT_TIME -= 60
        settings.LOCK_2_HINT = 1
        current_path = os.path.dirname(os.path.realpath(__file__))
        self.hint = customtkinter.CTkImage(Image.open(current_path + "/fur.png"),size=(300, 75))
        self.hint = customtkinter.CTkLabel(master = self.frame_riddle, image=self.hint, text = "")
        self.hint.grid(row = 5, column = 0)
        
    def complete(self):
        settings.LOCK_2_COMPLETE = 1
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Completed_Challenge(self.root)

class Lock_3(Frames):
    def __init__(self, root):
        super().set_state(6)
        settings.current_state = self.state
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "104!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

            try:
                _thread.start_new_thread( rx_data , (settings.MASTER_SERIAL, ))
            except:
                print ("Error: unable to start thread")

            try:
                _thread.start_new_thread( rx_data , (settings.ACCEL_BOARD, ))
            except:
                print ("Error: unable to start thread")

            
        #riddle frame
        self.frame_riddle = customtkinter.CTkFrame(master = root)
        self.frame_riddle.grid(row = 0, column = 0, sticky = "ns")

        #text
        # self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  "Solve the combination lock.",font = (self.font_text, 25), text_color = "light green")
        # self.riddle.grid(row = 0, column = 0, pady=10, padx=100)

        #images
        current_path = os.path.dirname(os.path.realpath(__file__))
        self.alpha = customtkinter.CTkImage(Image.open(current_path + "/position_1.png"),size=(150, 150))
        self.alpha = customtkinter.CTkLabel(master = self.frame_riddle, image=self.alpha, text = "")
        self.alpha.grid(row = 0, column = 0, pady = 10)
        self.alpha = customtkinter.CTkImage(Image.open(current_path + "/position_2.png"),size=(150, 150))
        self.alpha = customtkinter.CTkLabel(master = self.frame_riddle, image=self.alpha, text = "")
        self.alpha.grid(row = 0, column = 1, pady = 10)
        self.alpha = customtkinter.CTkImage(Image.open(current_path + "/position_3.png"),size=(150, 150))
        self.alpha = customtkinter.CTkLabel(master = self.frame_riddle, image=self.alpha, text = "")
        self.alpha.grid(row = 1, column = 0, pady = 10, columnspan = 2)


        #hint
        if settings.LOCK_3_HINT == 0:
            self.button_hint = customtkinter.CTkButton(master=self.frame_riddle, text="Help", width = 200, height = 50, hover = True, hover_color = "purple", command = self.hint, font = (self.font_text, 25))
            self.button_hint.grid(row = 5, column = 0, pady=12, padx=100, columnspan = 2)
        else:
            self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  "Rotate the secondary board to turn\nthe servos to the correct positions.",font = (self.font_text, 25), text_color = "light blue")
            self.riddle.grid(row = 5, column = 0, pady=10, padx=100, columnspan = 2)

        self.button_back = customtkinter.CTkButton(master=self.frame_riddle, text="Back", width = 200, height = 50, hover = True, hover_color = "blue", command = self.back, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        self.button_back.grid(row = 6, column = 0, pady=12, padx=100, columnspan = 2)
        
        if settings.TESTING == 1:
            self.complete_button = customtkinter.CTkButton(master=self.frame_riddle, text="Complete", width = 200, height = 50, hover = True, hover_color = "light green", command = self.complete, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
            self.complete_button.grid(row = 7, column = 0, pady=12, padx=200, columnspan = 2)

    def back(self):
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Open_Frame(self.root) 

    def hint(self):
        self.button_hint.destroy()
        settings.CURRENT_TIME -= 60
        settings.LOCK_3_HINT = 1
        self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  "Rotate the secondary board to turn\nthe servos to the correct positions.",font = (self.font_text, 25), text_color = "light blue")
        self.riddle.grid(row = 5, column = 0, pady=10, padx=100, columnspan = 2)
    
    def complete(self):
        settings.LOCK_3_COMPLETE = 1
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Completed_Challenge(self.root)

class ENC(Frames):
    def __init__(self, root):
        super().set_state(3)
        settings.current_state = self.state
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())


        #riddle frame
        self.frame_riddle = customtkinter.CTkFrame(master = root)
        self.frame_riddle.grid(row = 0, column = 0, sticky = "ns")

        #text
        self.riddle = customtkinter.CTkLabel(master = self.frame_riddle, text =  "You have been detected and the \nsystem as gone into lockdown. Solve \nthe encryption to enter the vault \nand take your prize.",font = (self.font_text, 25), text_color = "red")
        self.riddle.grid(row = 0, column = 0, pady=10, padx=50)


        self.button_log = customtkinter.CTkButton(master=self.frame_riddle, text="Login", width = 200, height = 50, hover = True, hover_color = "blue", command = self.login, font = (self.font_text, 25), fg_color = "DeepSkyBlue4")
        self.button_log.grid(row = 6, column = 0, pady=12, padx=50)

        # self.status = settings.SENTINAL + "101!"
        # settings.MASTER_SERIAL.write(str(self.state).encode())

    def login(self):
        self.frame_riddle.destroy()
        settings.LOGIN_SYSTEM = Login_Frame(self.root) 
    
class Winning(Frames):
    def __init__(self, root):
        super().set_state(7)
        
        settings.current_state = self.state
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

        self.winning_frame = customtkinter.CTkFrame(master = root)
        self.winning_frame.grid(row = 0, column = 0, sticky = "ns")

        self.msg = customtkinter.CTkLabel(master = self.winning_frame, text =  "Congratulations!",font = (self.font_text, 30), text_color = "white")
        self.msg.grid(row = 1, column = 0, pady=10, padx=150)

        self.msg = customtkinter.CTkLabel(master = self.winning_frame, text =  "You can clam the prize!",font = (self.font_text, 30), text_color = "white")
        self.msg.grid(row = 2, column = 0, pady=10, padx=150)

        self.seconds = settings.CURRENT_TIME%60
        self.minutes = settings.CURRENT_TIME//60

        self.timer = customtkinter.CTkLabel(master = self.winning_frame, text =  '',font = (self.font_text, 30), text_color = "white")
        self.timer.grid(row = 3, column = 0, pady=10, padx=150)

        self.timer = customtkinter.CTkLabel(master = self.winning_frame, text =  'You did it with',font = (self.font_text, 30), text_color = "white")
        self.timer.grid(row = 4, column = 0, pady=10, padx=150)

        self.timer = customtkinter.CTkLabel(master = self.winning_frame, text =  f'{self.minutes:{"0"}{2}} : {self.seconds:{"0"}{2}}',font = (self.font_text, 30), text_color = "red")
        self.timer.grid(row = 5, column = 0, pady=10, padx=150)

        self.timer = customtkinter.CTkLabel(master = self.winning_frame, text =  f'remaining.',font = (self.font_text, 30), text_color = "white")
        self.timer.grid(row = 6, column = 0, pady=10, padx=150)

        self.end_button = customtkinter.CTkButton(master=self.winning_frame, width=20,height=80, text="Exit", font = (self.font_text,30),hover_color = "red", hover = True, command = self.exit, fg_color = "DeepSkyBlue4")
        self.end_button.grid(row=8, column=0, padx=20, pady=50, sticky="ew")

    def exit(self):
        self.root.destroy()

class Lost(Frames):
    def __init__(self, root):
        
        super().set_state(8)
        settings.current_state = self.state
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

        self.winning_frame = customtkinter.CTkFrame(master = root)
        self.winning_frame.grid(row = 0, column = 0, sticky = "ns")

        self.msg = customtkinter.CTkLabel(master = self.winning_frame, text =  "You were captured!",font = (self.font_text, 30), text_color = "red")
        self.msg.grid(row = 1, column = 0, pady=10, padx=150)

        self.msg = customtkinter.CTkLabel(master = self.winning_frame, text =  "You Lose!",font = (self.font_text, 30), text_color = "red")
        self.msg.grid(row = 2, column = 0, pady=10, padx=150)

        self.end_button = customtkinter.CTkButton(master=self.winning_frame, width=20,height=80, text="Exit", font = (self.font_text,30),hover_color = "red", hover = True, command = self.exit, fg_color = "DeepSkyBlue4")
        self.end_button.grid(row=3, column=0, padx=20, pady=150, sticky="ew")

    def exit(self):
        self.root.destroy()
 
class Completed_Challenge(Frames):
    def __init__(self, root):
        super().set_state(8)
        settings.current_state = self.state
        self.root = root

        if settings.ENABLE_SERIAL == 1:
            self.status = settings.SENTINAL + "100!"
            settings.MASTER_SERIAL.write(self.status.encode())
            settings.TOUCH_BOARD.write(self.status.encode())
            settings.ACCEL_BOARD.write(self.status.encode())
            settings.ADC_BOARD.write(self.status.encode())

        self.completed_frame = customtkinter.CTkFrame(master = root)
        self.completed_frame.grid(row = 0, column = 0, sticky = "ns")

        self.msg = customtkinter.CTkLabel(master = self.completed_frame, text =  "Challenge Completed",font = (self.font_text, 30), text_color = "white")
        self.msg.grid(row = 1, column = 0, pady=10, padx=150)

        self.back_button = customtkinter.CTkButton(master=self.completed_frame, width=20,height=80, text="Back", font = (self.font_text,30),hover_color = "green", hover = True, command = self.back, fg_color = "DeepSkyBlue4")
        self.back_button.grid(row=3, column=0, padx=20, pady=50, sticky="ew")


    def back(self):
        self.completed_frame.destroy()
        settings.LOGIN_SYSTEM = Open_Frame(self.root) 

        
        
       

