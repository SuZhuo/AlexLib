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
    #time.sleep(0.1)


def click(cor):
    print("click --------------------- click")
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(cor[0], cor[1], \
            cor[0], cor[1], press_time)
    print(cmd)
    os.system(cmd)
    #time.sleep(0.1)

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
    cor = [1014, 1014]
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
    #time.sleep(0.1)

def slide2left():
    print("slide to left --------------------- slide to left")
    press_time = 200
    cmd = 'adb shell input swipe {0} {1} {2} {3} {4}'.format(238, 1440, \
            549, 1440, press_time)
    print(cmd)
    os.system(cmd)
    #time.sleep(0.1)

def inputText(s):
    
    cmd = 'adb shell am broadcast -a ADB_INPUT_TEXT --es msg \'{}\''.format(s)
    print(cmd)
    os.system(cmd)
    #time.sleep(0.1)

# 确定表情和英文单词数量
def getM():
    a = random.uniform(0,1)
    if(a < 0.10):
        M = 4
    elif(a < 0.45):
        M = 3
    elif(a < 0.8):
        M = 5
    elif(a < 0.88):
        M = 2
    elif(a < 0.96):
        M = 1
    else:
        M = 0

    b = random.uniform(0,1)
    if(b < 0.3):
        M2 = 0
    elif(b < 0.7):
        M2 = 1
    elif(b < 0.85):
        M2 = 2
    else:
        M2 = 3
    print('the number of emoji and english words are :', M, M2)
    return [M, M2]

# 确定文字数量
def getN():
    a = random.uniform(0, 1)
    if(a < 0.5):
        N = random.randint(1,20)
    else:
        N = random.randint(21, 35)
    print('the number of words: ', N)
    return N

def getSteps(e_M, w_M, N):
    M = e_M + w_M
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
    a.append(w_M)
    a.append(N)
    return a

def insertWords(ws, cur_w_ind):
    word_insert_n = random.randint(1,3)
    word_txt = ''
    for wk in range(word_insert_n):
        word_txt = '{0} {1}'.format(word_txt, ws[cur_w_ind])
        cur_w_ind += 1
    #inputText(word_txt)
    return [cur_w_ind, word_txt]


def operate(f, ws, gt_name, steps, cur_ind, cur_w_ind, axis):
    total = steps[-1]
    word_num = steps[-2]
    del steps[-1]
    del steps[-1]
    n = len(steps)
    try:
        word_pos = random.sample(range(n), word_num)
    except:
        word_pos = []

    pos = 0
    ocr_content = ''
    for i in range(n):
        if(steps[i] == pos):
            if i in word_pos:
                [cur_w_ind, word_txt] = insertWords(ws, cur_w_ind)
                ocr_content = '{0}{1}'.format(ocr_content, word_txt)
            else:
                [cur_ind, emoji_pos] = insertEmoj(cur_ind, axis)
                ocr_content = '{0}[emoji:{1},{2}]'.format(ocr_content, cur_ind, emoji_pos)
                a = random.uniform(0,1)
                if(a > 0.5):
                    [cur_ind, emoji_pos] = insertEmoj(cur_ind, axis)
                    ocr_content = '{0}[emoji:{1},{2}]'.format(ocr_content, cur_ind, emoji_pos)
                a = random.uniform(0,1)
                if(a < 0.2):
                    [cur_ind, emoji_pos] = insertEmoj(cur_ind, axis)
                    ocr_content = '{0}[emoji:{1},{2}]'.format(ocr_content, cur_ind, emoji_pos)
        else:
            charn = steps[i] - pos 
            s = f.read(charn)
            inputText(s)
            ocr_content = '{0}{1}'.format(ocr_content, s)
            pos = steps[i]
            if i in word_pos:
                [cur_w_ind, word_txt] = insertWords(ws, cur_w_ind)
                ocr_content = '{0}{1}'.format(ocr_content, word_txt)
            else:
                [cur_ind, emoji_pos] = insertEmoj(cur_ind, axis)
                ocr_content = '{0}[emoji:{1},{2}]'.format(ocr_content, cur_ind, emoji_pos)
                a = random.uniform(0,1)
                if(a > 0.7):
                    [cur_ind, emoji_pos] = insertEmoj(cur_ind, axis)
                    ocr_content = '{0}[emoji:{1},{2}]'.format(ocr_content, cur_ind, emoji_pos)
                a = random.uniform(0,1)
                if(a < 0.1):
                    [cur_ind, emoji_pos] = insertEmoj(cur_ind, axis)
                    ocr_content = '{0}[emoji:{1},{2}]'.format(ocr_content, cur_ind, emoji_pos)
    charn = total - pos 
    if(charn > 0):
        s = f.read(charn)
        inputText(s)
        ocr_content = '{0}{1}'.format(ocr_content, s)
    outf = open(gt_name, 'a')
    outf.write('{0}{1}'.format(ocr_content,'\n\n'))
    outf.close()
    print('send ----------------------------------------------- send')
    send()
    return [cur_ind, cur_w_ind]

def insertEmoj(cur_ind, axis):
    #makeEmoj()
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
    return [ind, apos]

def save_screen():
    push()
    pull_screenshot()
    pull()

def step_send_words(f, ws, gt_name, cur_w_ind):
    char_num_init = getN()
    s = f.read(char_num_init)
    char_num = len(s)
    words_num = math.ceil(char_num * 0.3)
    words_pos = random.sample(range(char_num), words_num)
    words_pos.sort()
    s_out = ''
    cur_pos = 0
    for i in words_pos:
        s_out += s[cur_pos : i]
        cur_pos = i 
        [cur_w_ind, word_txt] = insertWords(ws, cur_w_ind)
        s_out += word_txt 
    s_out += s[cur_pos:]
    
    inputText(s_out)
    outf = open(gt_name, 'a')
    outf.write('{0}{1}'.format(s_out,'\n\n'))
    outf.close()
    print('send: ', s_out)
    send()
    return [cur_w_ind, char_num_init] 


def main():
    f = open('jinyong.txt')
    # total: 1213198 
    f_pos = 135000
    f.read(f_pos)
    gt_name = '/home/alex/pictures/emoji_screen/pure_words_ocr_gt_4.txt'
    ef = open('jiaofu2.txt')
    ws = ef.read().split()
    #total: 176129 
    ws_pos = 33000
    ws = ws[ws_pos:]
    ef.close()

    cur_w_ind = 0
    char_total_num = 0

    icount = 0
    try:
        while True:
            icount += 1
            print("processing --------------- ", icount)
            cur_w_ind, char_num = step_send_words(f, ws, gt_name, cur_w_ind)
            char_total_num += char_num
            time.sleep(1.0)
    except KeyboardInterrupt:
        print('except in python: KeyboardInterrupt')

    print("quit job")
    f.close()

    info = 'next f_pos: {0}, next ws_pos: {1}'.format(f_pos + char_total_num, ws_pos + cur_w_ind)
    print(info)

    ocr_txt = open(gt_name, 'a')
    ocr_txt.write(info)


if __name__ == '__main__':
    main()
