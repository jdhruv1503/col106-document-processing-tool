import subprocess
import time

# Start time


# Run the C++ program
subprocess.run(["g++", "dict.cpp", "-o", "output"])

start_time = time.time()
subprocess.run(["./output"])

# End time
end_time = time.time()

# Calculate and print the execution time
execution_time = end_time - start_time
print(f"Execution time: {execution_time} seconds")
