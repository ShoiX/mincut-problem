name = raw_input("file to be cleaned: ") 
out = raw_input("name of new file: ")
handle1 = open(name, "r")
handle2 = open(out, "w+")

for line in handle1:
    words = line.split()
    n = len(words)
    handle2.write(str(words[0]) + " " + str(n-1) + " ")
    for i in range(1, n):
        handle2.write(str(words[i]) + " ")
    handle2.write("\n")
    
handle1.close()
handle2.close()
