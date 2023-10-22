import string

with open("in.txt", "r") as infile:
   inlist = infile.readlines()

mobydick = ""
for i in inlist:
    mobydick += i + " "

new_string = mobydick.translate(str.maketrans(' ', ' ', string.punctuation))
new_string2 = new_string.translate(str.maketrans('—', ' '))
new_string = new_string2.translate(str.maketrans(';', ' '))
new_string2 = new_string.translate(str.maketrans('"', ' '))
new_string = new_string2.translate(str.maketrans("'", ' '))
new_string2 = new_string.translate(str.maketrans('”', ' '))
new_string = new_string2.translate(str.maketrans("“", ' '))
new_string2 = new_string.translate(str.maketrans('’', ' '))
new_string = new_string2.translate(str.maketrans("“", ' '))

finalst = ' '.join(new_string.split())

with open("corpus.txt", "w") as corpus:
    corpus.write(finalst)
