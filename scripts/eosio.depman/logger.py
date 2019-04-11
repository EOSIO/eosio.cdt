import os, sys

colorize = True

def set_log_colorize( should_colorize ):
    global colorize
    colorize = should_colorize

class logger:
    red    = "91;1"
    green  = "92"
    yellow = "93;1"
    blue   = "94;1"
    should_exit = False
    silence     = False
    color = ""

    def __init__(self, c, silence, se):
        self.color = c
        self.silence = silence
        self.should_exit = se
        
    def log(self, s):
        global colorize
        if not self.silence:
            if colorize: 
                print("\33["+self.color+"m"+s+"\33[0m")
            else:
                print(s)
        if self.should_exit:
            exit(-1)

log          = logger(logger.green, False, False)
verbose_log  = logger(logger.blue, True, False)
warn         = logger(logger.yellow, False, False)
err          = logger(logger.red, False, True)
