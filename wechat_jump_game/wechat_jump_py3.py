import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from PIL import Image
import math
import time
import os

def pull_screenshot():
    os.system('adb shell screencap -p /sdcard/1.png')
    os.system('adb pull /sdcard/1.png .')

def click(cor):
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(cor[0], cor[1], \
            cor[0], cor[1], press_time)
    print(cmd)
    os.system(cmd)

def slide2right():
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(549, 1440, \
            238, 1440, press_time)
    print(cmd)
    os.system(cmd)

def slide2left():
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(238, 1440, \
            549, 1440, press_time)
    print(cmd)
    os.system(cmd)

def inputText(s):
    

    cmd = 'adb shell am broadcast -a ADB_INPUT_TEXT --es msg \'{}\''.format(s)

    print(cmd)
    os.system(cmd)

fig = plt.figure()
index = 0

pull_screenshot()
img = np.array(Image.open('1.png'))

update = True 
click_count = 0

def update_data():
    return np.array(Image.open('1.png'))

im = plt.imshow(img, animated=True)


def updatefig(*args):
    global update
    if update:
        time.sleep(1.5)
        pull_screenshot()
        im.set_array(update_data())
        update = False
    return im,

def onClick(event):      
    global update    
    global ix, iy

    # next screenshot
    
    ix, iy = event.xdata, event.ydata
    cor = [ix, iy]
    print('now = ', cor)
    
    #click(cor)
    #slide2right()
    inputText("请输入中文, ha,ha")
    update = True
        


fig.canvas.mpl_connect('button_press_event', onClick)
ani = animation.FuncAnimation(fig, updatefig, interval=50, blit=True)
plt.show()
