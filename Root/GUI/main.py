import customtkinter
import serial
import settings
import Frames


customtkinter.set_appearance_mode("dark")
customtkinter.set_default_color_theme("green")

settings.init() #innitialze global state

def run():
    root.update()
    


###############
#MAIN FUNCTION#
###############

root = customtkinter.CTk()
root.geometry("900x600")
settings.LOGIN_SYSTEM = Frames.Loading_Frame(root)





while(1):
    run()
    #update loading screan
    if settings.current_state == 0:
        settings.LOGIN_SYSTEM.update()

    #update timer
    if type(settings.LOGIN_SYSTEM) == Frames.Open_Frame:
        settings.LOGIN_SYSTEM.timer_update()
    
    #fail when timer finishes
    if settings.CURRENT_TIME <= 0 and type(settings.LOGIN_SYSTEM) != Frames.Lost and type(settings.LOGIN_SYSTEM) != Frames.Winning:
        settings.LOGIN_SYSTEM = Frames.Lost(root)


    #show completeion
    if settings.LOCK_1_COMPLETE == 1 and type(settings.LOGIN_SYSTEM) == Frames.Lock_1:
        settings.LOGIN_SYSTEM = Frames.Completed_Challenge(root)
    if settings.LOCK_2_COMPLETE == 1 and type(settings.LOGIN_SYSTEM) == Frames.Lock_2:
        settings.LOGIN_SYSTEM = Frames.Completed_Challenge(root)
    if settings.LOCK_3_COMPLETE == 1 and type(settings.LOGIN_SYSTEM) == Frames.Lock_3:
        settings.LOGIN_SYSTEM = Frames.Completed_Challenge(root)
    
    if type(settings.LOGIN_SYSTEM) == Frames.Lock_1:
        settings.LOGIN_SYSTEM.bar_update()
    
    if type(settings.LOGIN_SYSTEM) == Frames.Login_Frame:
        settings.LOGIN_SYSTEM.update_enc()

    if (settings.PRES_FAIl == 1 and type(settings.LOGIN_SYSTEM) != Frames.Lost):
        settings.LOGIN_SYSTEM = Frames.Lost(root)
    
    