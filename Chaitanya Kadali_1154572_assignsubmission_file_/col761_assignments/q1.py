import matplotlib.pyplot as plt

import sys

outp = sys.argv[1]

fsg_time = [0,0,0,0,0]
gaston_time = [0,0,0,0,0]
gspan_time = [0,0,0,0,0]

with open("Dump/time_gaston.txt", "r") as file:
    data = file.readlines()
    t1 = data[0].split()
    for i in range(len(t1)):
        gaston_time[len(t1)-1-i] = float(t1[i])

with open("Dump/time_fsg.txt", "r") as file:
    data = file.readlines()
    t1 = data[0].split()
    for i in range(len(t1)):
        fsg_time[len(t1)-1-i] = float(t1[i])

with open("Dump/time_gspan.txt", "r") as file:
    data = file.readlines()
    t1 = data[0].split()
    for i in range(len(t1)):
        gspan_time[len(t1)-1-i] = float(t1[i])



x = [5, 10, 25, 50, 95]


plt.plot(x, gaston_time, color = 'r', label = "Gaston")
plt.plot(x, fsg_time, color = 'b', label = "FSG")
plt.plot(x, gspan_time, color = 'g', label = "GSpan")


plt.xlabel("Support")
plt.ylabel("Running Times in seconds")
plt.title("Plot of Support threshold vs running times for different algorithms")

plt.legend()

plt.savefig(outp + ".png")

