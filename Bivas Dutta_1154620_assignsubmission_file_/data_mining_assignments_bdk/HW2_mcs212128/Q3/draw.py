# from cProfile import label
from sklearn.cluster import KMeans
import numpy as np
import matplotlib.pyplot as plt
import sys
def file_read(name):
    fi=open(name)
    datas=[]
    while True:
        x=fi.readline()
        if not x:
            return datas
        x=x.split()
        m=[]
        for i in x:
            m.append(float(i))
        datas.append(m)
fil=sys.argv[1]
def variations(data):
    data=np.array(data)
    output=[]
    for k in range(1,16):
        model=KMeans(n_clusters=k,random_state=0).fit(data)
        var=model.inertia_
        output.append(var)
    return output
def plotting(output):
    plt.xlabel('Values of K')
    plt.ylabel('Variations')
    plt.title("Elbow plot")
    plt.plot(output,marker='x',label='Variation Vs. K')
    plt.legend()
    plt.savefig(sys.argv[2])
datas=file_read(fil)
op=variations(datas)
plotting(op)
# print(datas)



