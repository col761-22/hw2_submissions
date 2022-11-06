import numpy as np
import matplotlib.pyplot as plt

fsg_times = np.loadtxt("fsg_times.txt")
gspan_times = np.loadtxt("gspan_times.txt")
gaston_times = np.loadtxt("gaston_times.txt")

supports = [5, 10, 25, 50, 95]

plt.plot(supports, fsg_times, label = "fsg")
plt.plot(supports, gaston_times, label = "gaston")
plt.plot(supports, gspan_times, label = "gspan")
plt.xlabel("support")
plt.ylabel("time (seconds)")
plt.legend()
plt.savefig("measured_times.png")