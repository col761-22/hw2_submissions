import sys
from sklearn.cluster import KMeans
from matplotlib import pyplot as plt

def kmeansGenerateElbowPlot(dataset,dimension,outputfile):
	points=[]
	clusterVariation=[]
	dataset=open(dataset,"r")
	for line in dataset:
		line=line.strip()
		data=line.split(' ')
		points.append([float(j) for j in data])
	for i in range(1, 16):
	    kmeans = KMeans(n_clusters = i)
	    kmeans.fit(points)
	    clusterVariation.append(kmeans.inertia_)
	
	noClusters=[i for i in range(1,16)]
	    
	plt.plot(noClusters, clusterVariation,'r*-')
	plt.xlabel('Number of Clusters')
	plt.ylabel('Variation within Clusters')
	plt.title('Elbow Plot')
	plt.savefig(outputfile)

if __name__=="__main__":
	dataset=sys.argv[1]
	dimension=int(sys.argv[2])
	outputfile=sys.argv[3]
	kmeansGenerateElbowPlot(dataset,dimension,outputfile)

