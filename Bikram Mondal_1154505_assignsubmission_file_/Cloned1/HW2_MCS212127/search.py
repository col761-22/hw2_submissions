import sys
from sortedcontainers import SortedDict
import time,os

graphs={}
querygraphs={}
dagMap={}

class Graph:
    def __init__(self,id,noVertex):
        self.id=id
        self.noVertex=noVertex
        self.vlabels={}
        self.adj={}
        self.edges={}

class Node:
    def __init__(self):
        self.gid=[]
        self.children=set()
        self.canLab=""

root=Node()
root.canLab="$"
dagMap["$"]=root

def createGraphs(inputfile):
    inputfile=open(inputfile,'r')
    while True:
        line = inputfile.readline()
        if not line:
            break
        line=line.strip()
        if len(line)==0:
            continue
        if line[0]=='#':
            nextLine=inputfile.readline().strip()
            if len(nextLine)==0:
                continue
            v=int(nextLine)
            nextGraph=Graph(line,v)
            for i in range(v):
                nextLine=inputfile.readline().strip()
                nextGraph.vlabels[i]=nextLine
            nextLine=inputfile.readline().strip()
            if len(nextLine)==0:
                graphs.append(nextGraph)
                continue
            e=int(nextLine)
            for i in range(e):
                nextLine=inputfile.readline().strip().split()
                v1=int(nextLine[0])
                v2=int(nextLine[1])
                edgeLebel=nextLine[2]
                if v1 not in nextGraph.adj:
                    nextGraph.adj[v1]=[(v2,edgeLebel)]
                else:
                    nextGraph.adj[v1].append((v2,edgeLebel))
                if v2 not in nextGraph.adj:
                    nextGraph.adj[v2]=[(v1,edgeLebel)]
                else:
                    nextGraph.adj[v2].append((v1,edgeLebel))
                nextGraph.edges[(v1,v2)]=edgeLebel
                nextGraph.edges[(v2,v1)]=edgeLebel
            graphs[nextGraph.id]=nextGraph
    inputfile.close()

def createQueryGraph(inputfile):
    inputfile=open(inputfile,'r')
    while True:
        line = inputfile.readline()
        if not line:
            break
        line=line.strip()
        if len(line)==0:
            continue
        if line[0]=='#':
            nextLine=inputfile.readline().strip()
            if len(nextLine)==0:
                continue
            v=int(nextLine)
            nextGraph=Graph(line,v)
            for i in range(v):
                nextLine=inputfile.readline().strip()
                nextGraph.vlabels[i]=nextLine
            nextLine=inputfile.readline().strip()
            if len(nextLine)==0:
                graphs.append(nextGraph)
                continue
            e=int(nextLine)
            for i in range(e):
                nextLine=inputfile.readline().strip().split()
                v1=int(nextLine[0])
                v2=int(nextLine[1])
                edgeLebel=nextLine[2]
                if v1 not in nextGraph.adj:
                    nextGraph.adj[v1]=[(v2,edgeLebel)]
                else:
                    nextGraph.adj[v1].append((v2,edgeLebel))
                if v2 not in nextGraph.adj:
                    nextGraph.adj[v2]=[(v1,edgeLebel)]
                else:
                    nextGraph.adj[v2].append((v1,edgeLebel))
                nextGraph.edges[(v1,v2)]=edgeLebel
                nextGraph.edges[(v2,v1)]=edgeLebel
            querygraphs[nextGraph.id]=nextGraph

def findCanonical(g):
    lebel=""
    invMap=SortedDict()
    vertexInvariant={}
    vertexOrder=[]
    for v in g.vlabels:
        vertexInvariant[v]=[]
        for tup in g.adj[v]:
            vertexInvariant[v].append((tup[1],g.vlabels[tup[0]],len(g.adj[tup[0]])))
        vertexInvariant[v].sort(key=lambda x:(x[0],x[1],x[2]))
        if tuple(vertexInvariant[v]) in invMap:
            invMap[tuple(vertexInvariant[v])].append([v,g.vlabels[v]])
        else:
            invMap[tuple(vertexInvariant[v])]=[[v,g.vlabels[v]]]
    for key in invMap:
        invMap[key].sort(key=lambda x:x[1])
    for key in invMap:
        for ve in invMap[key]:
            vertexOrder.append(ve[0])
            lebel+=ve[1]
    for i in range(g.noVertex):
        for j in range(i):
            if (vertexOrder[j],vertexOrder[i]) in g.edges:
                lebel+=g.edges[(vertexOrder[j],vertexOrder[i])]
            else:
                lebel+=''
    hash_lebel=0
    for i in lebel:
        '''temp=str(ord(i))
        for j in temp:
            hash_lebel+=int(j)'''
        hash_lebel+=ord(i)
    hash_lebel=hash_lebel%8192
    #hash_lebel=hash(lebel)%1024
    return str(hash_lebel)

def loadIndex(indexfile):
    indexfile=open(indexfile,'r')
    while True:
        line = indexfile.readline()
        if not line:
            break
        line=line.strip()
        if len(line)==0:
            continue
        if line[0]=='#':
            canLab=indexfile.readline().strip()
            if canLab=='$':
                newNode=root
            else:
                if canLab not in dagMap:
                    newNode=Node()
                    newNode.canLab=canLab
                    dagMap[canLab]=newNode
                else:
                    newNode=dagMap[canLab]
            gids=line.split()
            for id in gids:
                if id!='#':
                    newNode.gid.append(id)
            childs=indexfile.readline().strip()
            if childs=='-':
                continue
            childs=childs.split()
            for child in childs:
                if child in dagMap:
                    newNode.children.add(dagMap[child])
                else:
                    childNode=Node()
                    childNode.canLab=child
                    dagMap[child]=childNode
                    newNode.children.add(dagMap[child])

    indexfile.close()


def search(g,c):
    canLab=findCanonical(g)
    if canLab not in dagMap:
        return []
    res=[]
    visited={}
    q=[dagMap[canLab]]
    visited[q[0]]=True
    while(q):
        node=q.pop(0)
        for id in node.gid:
            res.append(id)
        for child in node.children:
            if child not in visited:
                visited[child]=True
                q.append(child)
    res=list(set(res))

    #update the result by checking 1-edges
    updatedRes=[]
    for id in res:
        flag=0
        mainGraph=graphs[id]
        d={}
        for tup in mainGraph.edges:
            d[(mainGraph.vlabels[tup[0]],mainGraph.vlabels[tup[1]],mainGraph.edges[tup])]=True
        for tup in g.edges:
            if (g.vlabels[tup[0]],g.vlabels[tup[1]],g.edges[tup]) not in d:
                flag=1
                break
        if flag==0:
            updatedRes.append(id)

    #store resultant graphs in file
    outputfile=open("q"+str(c)+".txt",'w')
    for gid in updatedRes:
        graph=graphs[gid]
        outputfile.write(graph.id+"\n")
        outputfile.write(str(graph.noVertex)+"\n")
        for nodelebel in graph.vlabels:
            outputfile.write(graph.vlabels[nodelebel]+"\n")
        outputfile.write(str(len(graph.edges)//2)+"\n")
        for edge in graph.edges:
            if edge[0]>edge[1]:
                outputfile.write(str(edge[0])+" "+str(edge[1])+" "+graph.edges[edge]+"\n")
        outputfile.write("\n")
    outputfile.close()

    return updatedRes

if __name__ == "__main__":
    inputfile=open("inputfile_BHK.txt",'r')
    infileName=inputfile.readline().strip()
    inputfile.close()

    createGraphs(infileName)

    loadIndex("indexfile_BHK.txt")
    print("Index Structure loaded")

    print("Enter name of query file: ",end=" ")
    queryFile = input()

    stTime=time.time()

    createQueryGraph(queryFile)
    tempfile=open("queryfile_BHK.txt",'w')
    tempfile.write(queryFile)
    tempfile.close()
    c=1
    for g in querygraphs:
        resIds=search(querygraphs[g],c)
        c+=1
        '''print(resIds)
        print(len(resIds))
        if querygraphs[g].id in resIds:
            print("Original present")'''
    os.system(f"./a.out")       

    print("Time required: ",(time.time()-stTime)*1000," ms")