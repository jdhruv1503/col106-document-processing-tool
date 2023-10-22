import string
import os
import time
import subprocess
import shutil

corp = []

for dirpath, dirnames, filenames in os.walk('./txtfiles'):
    for file in filenames:
        # Check if file is a C++ file
        if file.endswith('.txt'):
            # Full path to the file
            filepath = os.path.join(dirpath, file)
            with open(filepath, "r") as gandhi:
                corp.extend(gandhi.readlines())

mobydick = ""
for i in corp:
    mobydick += i[i.find(")")+1:] + " "
    print("add", i[:i.find(")")+1])

finalst = ' '.join(mobydick.split())

with open("corpus.txt", "w") as corpus:
    corpus.write(finalst)
