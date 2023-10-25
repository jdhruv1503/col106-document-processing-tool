import os
import time
import subprocess
import shutil

# List to store results
results = []
TIMES_EXEC = 5

# Walk through all subdirectories
for dirpath, dirnames, filenames in os.walk('.'):
    for file in filenames:
        # Check if file is a C++ file
        if file.endswith('search.cpp'):
            # Full path to the file
            filepath = os.path.join(dirpath, file)
            # Name of the executable
            exec_name = os.path.join(dirpath, 'exec')
            shutil.copy('corpus.txt', dirpath)
            
            # Compile the C++ file
            subprocess.run(['g++', "Naive-Vector-Push/Node.cpp", filepath, '-o', exec_name])
            
            # Time the execution of the program

# Warmup
# Run the C++ program
subprocess.run(["g++", "Naive-Vector-Push/Node.cpp", "Naive-Vector-Push/search.cpp", "-o", "output"])

for _ in range(8):
    subprocess.run(['./output'])
    print(_)
os.remove('./output')

print("Warmed up")

for dirpath, dirnames, filenames in os.walk('.'):
    for file in filenames:    
        if file.endswith('exec'):
            exec_name = os.path.join(dirpath, file)
            start = time.time()
            for _ in range(TIMES_EXEC):
                subprocess.run([exec_name])
            end = time.time()
            
            # Calculate the total execution time
            exec_time = end - start
            
            # Store the result
            results.append((dirpath, exec_time))
            print("Executed ",dirpath)
            
            # Remove the executable
            os.remove(exec_name)
            os.remove(os.path.join(dirpath, 'corpus.txt'))


# Generate the text file
with open('results - gandhi.txt', 'w') as f:
    for dirpath, exec_time in results:
        f.write(f'{dirpath}: {exec_time} seconds\n')