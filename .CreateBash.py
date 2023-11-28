import os
import stat


def bashsrc(i: int) -> str:
    return f'''#!/bin/bash
gcc ex{i}.c -o ex{i}.out -Wextra -Wpedantic
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi
./ex{i}.out
rm ex{i}.out
'''


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
            f.write(bashsrc(i))
            f.flush()
            f.close()
        os.chmod(shname, stat.S_IMODE(os.stat(shname).st_mode | stat.S_IEXEC))
    except FileExistsError:
        print('File exists!')

print('Done')
