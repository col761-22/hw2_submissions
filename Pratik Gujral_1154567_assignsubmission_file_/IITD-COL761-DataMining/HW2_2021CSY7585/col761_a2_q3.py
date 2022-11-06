#!/usr/bin/env python
# coding: utf-8

import pandas as pd
import sys
from matplotlib import pyplot as plt
# from sklearn.datasets.samples_generator import make_blobs
from sklearn.cluster import KMeans


# DATASET_PATH = r"C:\Users\prati\Downloads\hw2_updated\hw2_updated\pbs.3237740.pbshpc.x8z"
DATASET_PATH = sys.argv[1]
DIMENSION = sys.argv[2]
PLOT_NAME = sys.argv[3]

# Read the dataset
data = pd.read_csv(DATASET_PATH, sep=' ', header=None)
k_values = [*range(1,16)]

wcss = []
for i in k_values:
    kmeans = KMeans(n_clusters=i, n_init=20, random_state=0)
    kmeans.fit(data)
    wcss.append(kmeans.inertia_)

plt.plot(k_values, wcss)
plt.title('Elbow Method')
plt.xlabel('Number of clusters')
plt.ylabel('Within-Cluster-Sum-of-Squares (WCSS)')
plt.savefig(f"q3_{DIMENSION}_CSY217585.png")