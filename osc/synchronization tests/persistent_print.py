import sys
import time
from datetime import datetime
import builtins as __builtin__
    
def print(*args, sep = ' '):
    # HOW TO IMPLEMENT END AND FLUSH?
    now = datetime.now()
    timestamp = datetime.timestamp(now)
    readable_timestamp = datetime.fromtimestamp(timestamp)

    __builtin__.print(*args, sep=sep, end='', flush=True)
    for i in range(100):
        sys.stdout.write(' ')
    __builtin__.print('')
    
    sys.stdout.write('\r')
    sys.stdout.write('[%s] Computer 1: ' % readable_timestamp)
    sys.stdout.write('\033[92mONLINE\033[0m' if c1==1 else '\033[91mOFFLINE\033[0m')
    sys.stdout.write(' | Computer 2: ')
    sys.stdout.write('\033[92mONLINE\033[0m' if c2==1 else '\033[91mOFFLINE\033[0m')
    sys.stdout.write('\r')
    sys.stdout.flush()

c1 = 0
c2 = 0
original=0

for i in range(10):
    
    if original==1:
        __builtin__.print("Doing something", i, i, 'werwerwer')
        __builtin__.print(i, end='')
        __builtin__.print(i)
        __builtin__.print(i)
    else:
        print("Doing something", i, i, 'werwerwer')
        print(i)
        print(i)
        print(i)

    c1=1

    time.sleep(0.5)