import os
import stat

week = 5
basepath = f'week{str(0) + str(week) if week < 10 else str(week)}/'
try:
    os.mkdir(basepath)
except FileExistsError:
    print('Folder already exists!')

for i in range(1,8):
    try:
        with open(f'{basepath}ex{i}.c', 'x') as f:
            f.write(open('week01/main.c').read())
            f.flush()
            f.close()
    except FileExistsError:
        print('File exists!')
    try:
        shname = f'{basepath}ex{i}.sh'
        with open(shname, 'x')as f:
            f.write(f'#!/bin/bash\ngcc ex{i}.c -o ex{i} -lm -lpthread -lrt\n./ex{i}\nrm ex{i}\n')
            f.flush()
            f.close()
        os.chmod(shname, stat.S_IMODE(os.stat(shname).st_mode | stat.S_IEXEC))
    except FileExistsError:
        print('File exists!')

print('Done')
