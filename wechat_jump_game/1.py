import random 
import math 

def getSteps(M, N):
    a = list(range(N+1))
    pad = math.ceil(N * 0.35)
    start = pad * [0]
    end = pad * [N]
    a.extend(start)
    a.extend(end)
    random.shuffle(a)
    if M > len(a):
        a.extend(a[:M - len(a)])
    return a

#a = getSteps(5,2)
#b = getSteps(5,1)
#print(a)
#print(b)
x = [82, 238, 399, 549, 695, 856, 1012]
y = [1280, 1440, 1617]
pos = [[a, b] for a in x for b in y]
pos.remove([1012, 1617])
print(pos, len(pos))
