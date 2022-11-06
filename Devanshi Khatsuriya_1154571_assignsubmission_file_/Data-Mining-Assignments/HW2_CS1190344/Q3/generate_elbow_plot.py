import sys
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt


if len(sys.argv) != 4:
    print("Error: Incorrect Usage!\nUsage: python3 generate_elbow_plot.py <input_dataset_file> <dimension> <plot_name>")
    # example usage: python3 generate_elbow_plot.py CS1190327_generated_dataset_3D.dat 3 q3_3_CS1190327.png
    sys.exit(1)

input_dataset_file = sys.argv[1]
dimension = int(sys.argv[2])
fig_name = sys.argv[3]

points = np.empty([0, dimension], dtype=np.float64)

try:

    with open(input_dataset_file, "r") as f:
        line = f.readline()
        while line:
            # print(points)
            line = line.strip()
            point = np.array(list(map(np.float64, line.split())))
            if point.shape[0] != dimension:
                print("Error: Dimension of point is not equal to the given dimension!")
                sys.exit(1)
            point = point.reshape(1, dimension)
            points = np.concatenate((points, point), axis=0)
            line = f.readline()
    print(f"Shape of dataset: {points.shape}")

    k_values = []
    inertia_values = []

    for k in range(1, 16):
        print(f"\nRunning KMeans for k = {k}...")
        kmeans = KMeans(n_clusters=k, verbose=1, random_state=0).fit(points)
        # print(kmeans.labels_.shape)
        k_values.append(k)
        inertia_values.append(kmeans.inertia_)

    fig = plt.gcf()
    fig.set_size_inches(8, 6)

    plt.plot(k_values, inertia_values, linestyle='-', marker='.', label = "Elbow Plot")

    plt.title(f"Elbow Plot for Dataset of dimension={dimension}")

    plt.xlabel("k (Number of Clusters)")
    plt.ylabel("Inertia (Sum of Intracluster Distances)")
    plt.legend()
    plt.show()

    fig.savefig(fig_name, dpi=100)

    print(f"\nGenerated and saved the plot at {fig_name}")

except Exception as e:
    print(f"Error: {e}")
    sys.exit(1)