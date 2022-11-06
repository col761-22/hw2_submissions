import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
	print("Plot script did not get name of plot as argument.")
	sys.exit()

fig_name = sys.argv[1] + ".png"

algos = ["gspan", "fsg", "gaston"]
support_thresholds = [[], [], []]
timings = [[], [], []]

for index, algo in enumerate(algos):
    try:
        filename = algo + "_timings.txt"
        with open(filename, "r") as f:
            while True:
                values = f.readline().split()
                if not values:
                    break
                if values[0] == "Support": # matches the line of the form "Support Threshold d"
                    support_threshold = float(values[2])
                    while True:
                        values = f.readline().split()
                        if not values:
                            break
                        if values[0] == "Elapsed": # matches the line of the form "Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.ss"
                            denotations = values[-1].split(":")
                            if len(denotations) == 2: # means time was in h:mm:ss format
                                time_in_seconds = int(denotations[0]) * 60 + float(denotations[1])
                            elif len(denotations) == 3: # means time was in m:ss format
                                time_in_seconds = int(denotations[0]) * 3600 + int(denotations[1]) * 60 + float(denotations[2])
                            support_thresholds[index].append(support_threshold)
                            timings[index].append(time_in_seconds)
                            break
                if not values:
                    break
    except Exception as e:
        print(f"{e}. Skipping file '{filename}'...")

fig = plt.gcf()
fig.set_size_inches(8, 6)

print(support_thresholds, timings)

for index, algo in enumerate(algos):
    plt.plot(list(reversed(support_thresholds[index])), list(reversed(timings[index])), linestyle='-', marker='.', label=algo)

plt.xlabel("Support Threshold (Percentage)")
plt.ylabel("Time (Seconds)")
plt.legend()
plt.show()

fig.savefig(fig_name, dpi=100)