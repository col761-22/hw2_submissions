from tkinter import W
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import sys


filename = sys.argv[1]
image_name = sys.argv[2]

data = np.loadtxt(filename, delimiter=" ")

sse = {}

for k in range(1, 16):

    k_means = KMeans(n_clusters = k).fit(data)
    sse[k] = k_means.inertia_

elb = {}
for i in range(2, 15):
    elb[i] = (sse[i-1]-sse[i]) / (sse[i]-sse[i+1])

k_opt = max(elb, key = elb.get)

plt.figure()
plt.plot(list(sse.keys()), list(sse.values()), label = "metric")
plt.plot([k_opt], [sse[k_opt]], 'x', c = 'red', label = "elbow point")
plt.legend()
plt.xlabel("k")
plt.ylabel("variation within clusters (sum of squared distances to centroids)")
plt.title(f"elbow plot for {image_name}")
plt.savefig(image_name,dpi=300)
plt.show() 
