import subprocess
import time

def run(*popenargs, **kwargs):
    input = kwargs.pop("input", None)
    check = kwargs.pop("handle", False)

    if input is not None:
        if 'stdin' in kwargs:
            raise ValueError('stdin and input arguments may not both be used.')
        kwargs['stdin'] = subprocess.PIPE

    process = subprocess.Popen(*popenargs, **kwargs)
    try:
        stdout, stderr = process.communicate(input)
    except:
        process.kill()
        process.wait()
        raise
    retcode = process.poll()
    if check and retcode:
        raise subprocess.CalledProcessError(
            retcode, process.args, output=stdout, stderr=stderr)
    return retcode, stdout, stderr

# Start time
NUMBER_RUNS = 50


# Run the C++ program
run(["g++", "dict.cpp", "-o", "output"])

start_time = time.time()
for i in range(0,NUMBER_RUNS):
    run(["./output"])

# End time
end_time = time.time()

# Calculate and print the execution time
execution_time = end_time - start_time
print("Execution time: ",execution_time/NUMBER_RUNS," seconds")
