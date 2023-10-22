import string

with open("in.txt", "r") as infile:
   inlist = infile.readlines()

mobydick = ""
for i in inlist:
    mobydick += i + " "

finalst = ' '.join(mobydick.split())

with open("corpus.txt", "w") as corpus:
    corpus.write(finalst)
