import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from scipy.spatial.distance import cdist
import sys

dataset = sys.argv[1]
dimension = sys.argv[2]

plot = 'q3_' + dimension + '_CS5190435.png'

error = []
distortions = []
X = np.loadtxt(dataset) 

for k in range(1,16):
    k_means = KMeans(n_clusters = k)
    k_means.fit(X)
    #error.append(k_means.inertia_)
    distortions.append(sum(np.min(cdist(X, k_means.cluster_centers_,
                                        'euclidean'), axis=1)) / X.shape[0])

# plot errors

"""
plt.figure(figsize=(8, 6), dpi = 500)
plt.plot(range(1, 16), error, '-o', c = 'maroon')
plt.vlines(x= 9 , ymin = -10, ymax = 270, colors='green', linestyles='dotted', linewidth=3)
plt.xlabel('Number of clusters')
plt.ylabel('Inertia ( Sum of Squared Distances from Respective Closest Cluster Centres)')
plt.savefig(plot)

"""


#plot distortions


for i , k in enumerate(distortions): 
    if (i<len(distortions)-1):
        if distortions[i] - distortions[i+1]< 0.001:
            n_cluster = i+1
            break
  

kk = n_cluster 



plt.figure(figsize=(8, 6), dpi = 500)
plt.plot(range(1, 16), distortions, '-o', c = 'maroon')
plt.vlines(x= kk , ymin =  0 , ymax = max(distortions), colors='green', linestyles='dotted', linewidth=3)
plt.xlabel('Number of clusters')
plt.ylabel('Distortions (Average Euclidean Squared Distance From Respective Clusters)')
plt.annotate('Number of clusters is  ' + f"{kk}", xy = (kk+1,np.mean(distortions)) , xytext=(kk+1, np.mean(distortions)) ,color='red', fontsize=13)
plt.savefig(plot)



