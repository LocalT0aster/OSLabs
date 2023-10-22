import os
import re
import sys

# Extract exercise number from filename
filename = sys.argv[1]
match = re.match(r'ex(\d+)\.c', filename)
if match:
    exercise_num = match.group(1)
else:
    # If not in filename, try the first line comment
    with open(filename, 'r') as f:
        first_line = f.readline().strip()
        match = re.match(r'// ex(\d+)', first_line)
        if match:
            exercise_num = match.group(1)
        else:
            print("Error: Exercise number not found.")
            sys.exit(1)

script_name = f'ex{exercise_num}.sh'
if os.path.exists(script_name):
    os.system(f'chmod +x {script_name} && ./{script_name}')
else:
    print(f"Error: Script {script_name} not found.")
