import numpy as np
import pandas as pd
import statsmodels.api as sm
import matplotlib.pyplot as plt
import seaborn as sns
import sys
sns.set()
from sklearn.cluster import KMeans

inputfile = sys.argv[1] #r"C:\Users\apasingh.KEYSIGHT\Downloads\MCS212130_generated_dataset_4D.dat" #
Ndimensions = int(sys.argv[2]) #4 #
outputfile = sys.argv[3] #"q3_4_MCS212124.png" #
a=1
b=15
# loading dataset
datContent = [i.strip().split() for i in open(inputfile).readlines()]
df= pd.DataFrame()
# Create the pandas DataFrame
if Ndimensions == 2:
    df = pd.DataFrame(datContent, columns = ['Dimension 1', 'Dimension 2'])
elif Ndimensions == 3:
    df = pd.DataFrame(datContent, columns = ['Dimension 1', 'Dimension 2', 'Dimension 3'])
elif Ndimensions == 4:
    df = pd.DataFrame(datContent, columns = ['Dimension 1', 'Dimension 2', 'Dimension 3', 'Dimension 4'])
# checking data shape
row, col = df.shape
#print(f'There are {row} rows and {col} columns') 

#print(df.head(10))


#K-Means for different cluster sizes
elbow=[]
ans = []
for i in range(a,b+1):
    kmeans = KMeans(i)
    kmeans.fit(df)
    identified_clusters = kmeans.fit_predict(df)
    elbow_iter = kmeans.inertia_ 
    elbow.append(elbow_iter)
    ans.append((i, elbow_iter))
    
results = pd.DataFrame(ans, columns = ['k','Inertia']).set_index('k')
best_k = results.idxmin()[0]
print(best_k)
print(results)
number_clusters = range(a,b+1)
plt.plot(number_clusters,elbow, 'bx-')
plt.title('Elbow Method For Optimal k')
plt.xlabel('Number of clusters')
plt.ylabel('Sum of squared distances/Inertia')
plt.savefig(outputfile)


