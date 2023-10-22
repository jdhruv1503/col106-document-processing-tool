import os
import time
import subprocess

# Warmup
# Run the C++ program
subprocess.run(["g++", "dict.cpp", "-o", "output"])

print("compiled")

start = time.time()
subprocess.run(['./output'])
end = time.time()
            
            # Calculate the total execution time
exec_time = end - start
os.remove('./output')
print(f'{exec_time} seconds\n')