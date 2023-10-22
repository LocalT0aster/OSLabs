import os
import stat

print("Enter the number of week:")
week = int(input())
print("Enter number of shell scripts to create:")
n = int(input()) + 1
basepath = f'week{str(0) + str(week) if week < 10 else str(week)}/'
try:
    os.mkdir(basepath)
except FileExistsError:
    print('Folder already exists!')

for i in range(1,n):
    try:
        shname = f'{basepath}ex{i}.sh'
        with open(shname, 'x')as f:
            f.write(f'#!/bin/bash\ngcc ex{i}.c -o ex{i}.out -lm -Wextra -Wpedantic\n./ex{i}.out\nrm ex{i}.out\n')
            f.flush()
            f.close()
        os.chmod(shname, stat.S_IMODE(os.stat(shname).st_mode | stat.S_IEXEC))
    except FileExistsError:
        print('File exists!')

print('Done')
