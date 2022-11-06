import sys
import matplotlib.pyplot as plt

apriori_timing = open('apriori_timing.dat', 'r')
fptree_timing = open('fptree_timing.dat', 'r')

apriori = [i[:-1] for i in apriori_timing.readlines()]
fptree = [i[:-1] for i in fptree_timing.readlines()]

supports = [5, 10, 25, 50, 90]

plt.plot(supports, apriori, label = "apriori")
plt.plot(supports, fptree, label = "fptree")
plt.xlabel("support")
plt.ylabel("time")
plt.legend()
plt.savefig(sys.argv[1])