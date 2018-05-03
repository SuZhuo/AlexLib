# coding: utf-8
import os
import shutil
import time
import math
from PIL import Image, ImageDraw
import random


# === 思路 ===


screenshot_backup_dir = 'screenshot_backups/'
if not os.path.isdir(screenshot_backup_dir):
        os.mkdir(screenshot_backup_dir)


def pull_screenshot():
    os.system('adb shell screencap -p /sdcard/1.png')
    ts = int(time.time())
    os.system('adb pull /sdcard/1.png ./{0}{1}.png'\
            .format(screenshot_backup_dir, ts))
    time.sleep(0.1)


def click(cor):
    print("click --------------------- click")
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(cor[0], cor[1], \
            cor[0], cor[1], press_time)
    print(cmd)
    os.system(cmd)
    time.sleep(0.1)

def pull():
    print("pull --------------------- pull")
    cor = [851, 1835]
    click(cor)

def push():
    print("push --------------------- push")
    cor = [851, 1097]
    click(cor)

def send():
    print("send --------------------- send")
    cor = [991, 1092]
    click(cor)

def makeEmoj():
    cor = [274, 1876]
    click(cor)

def slide2right():

    print("slide to right --------------------- slide to right")
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(549, 1440, \
            238, 1440, press_time)
    print(cmd)
    os.system(cmd)
    time.sleep(0.1)

def slide2left():
    print("slide to left --------------------- slide to left")
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(238, 1440, \
            549, 1440, press_time)
    print(cmd)
    os.system(cmd)
    time.sleep(0.1)

def inputText(s):
    
    cmd = 'adb shell am broadcast -a ADB_INPUT_TEXT --es msg \'{}\''.format(s)
    print(cmd)
    os.system(cmd)
    time.sleep(0.1)

# 确定表情数量
def getM():
    a = random.uniform(0,1)
    if(a < 0.10):
        M = 0
    elif(a < 0.45):
        M = 1
    elif(a < 0.8):
        M = 2
    elif(a < 0.88):
        M = 3
    elif(a < 0.96):
        M = 4
    else:
        M = 5
    return M

# 确定文字数量
def getN():
    a = random.uniform(0, 1)
    if(a < 0.5):
        N = random.randint(0,15)
    else:
        N = random.randint(16, 50)
    return N

def getSteps(M, N):
    if(0 == M and 0 == N):
        ran = random.uniform(0,1)
        if(ran > 0.5):
            M = 1
        else:
            N = 1
    a = list(range(N+1))
    pad = math.ceil(N * 0.35)
    start = pad * [0]
    end = pad * [N]
    a.extend(start)
    a.extend(end)
    random.shuffle(a)
    if M > len(a):
        a.extend(a[:M - len(a)])
    a = a[:M]
    a.sort()
    a.append(N)
    return a

def operate(f, steps, cur_ind, axis):
    total = steps[-1]
    del steps[-1]
    n = len(steps)
    pos = 0
    for i in range(n):
        if(steps[i] == pos):
            cur_ind = insertEmoj(cur_ind, axis)
        else:
            charn = steps[i] - pos 
            s = f.read(charn)
            inputText(s)
            pos = steps[i]
            cur_ind = insertEmoj(cur_ind, axis)
    charn = total - pos 
    if(charn > 0):
        s = f.read(charn)
        inputText(s)
    send()
    return cur_ind

def insertEmoj(cur_ind, axis):
    makeEmoj()
    ind = random.randint(0,4)
    if(ind < cur_ind):
        clips = cur_ind - ind;
        for i in range(clips):
            slide2left()
    else:
        clips = ind - cur_ind;
        for i in range(clips):
            slide2right()
    apos = random.randint(0, len(axis) - 1)
    cor = axis[apos]
    click(cor)
    return ind

def save_screen():
    push()
    pull_screenshot()
    pull()

def main():
    f = open('jinyong.txt')
    x = [82, 238, 399, 549, 695, 856, 1012]
    y = [1280, 1440, 1617]
    axis = [[a, b] for a in x for b in y]
    axis.remove([1012, 1617])
    cur_ind = 0

    capture = 0
    icount = 0
    try:
        while True:
            icount += 1
            print("processing --------------- ", icount)
            capture += 1
            char_num = getN()
            emo_num = getM()
            steps = getSteps(emo_num, char_num)
            cur_ind = operate(f, steps, cur_ind, axis)
            if(4 == capture):
                save_screen()
                capture = 0
    except KeyboardInterrupt:
        print('except in python: KeyboardInterrupt')

    print("quit job")
    f.close()


if __name__ == '__main__':
    main()
