import sys
#from sortedcontainers import SortedDict
import copy

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

def storeIndex():
    indexfile=open("indexfile_BHK.txt",'w')
    visited={}
    q=[root]
    visited[q[0]]=True
    while(q):
        node=q.pop(0)
        if len(node.gid)==0:
            indexfile.write("#")
        for id in node.gid:
            indexfile.write(id)
            indexfile.write(" ")
        indexfile.write("\n")
        indexfile.write(node.canLab+"\n")
        if len(node.children)==0:
            indexfile.write("-")
        for child in node.children:
            if child not in visited:
                visited[child]=True
                q.append(child)
            indexfile.write(child.canLab)
            indexfile.write(" ")
        indexfile.write("\n")
        indexfile.write("\n")
    indexfile.close()

def printGraphs():
    for g in graphs:
        print(graphs[g].id,graphs[g].noVertex)
        print(graphs[g].vlabels)
        print(graphs[g].adj)

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

def findCanonical(g):
    lebel=""
    invMap={}
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
    for key in sorted(invMap):
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

def dfs(g,v,visited):
    visited[v]=True
    for u in g.adj[v]:
        if visited[u[0]]==False:
            dfs(g,u[0],visited)

def isConnected(g):
    visited={}
    for v in g.vlabels:
        visited[v]=False
    for v in g.vlabels:
        st=v
        break
    dfs(g,st,visited)
    for i in visited:
        if visited[i]==False:
            return False
    return True

def insertSubgraphs(g,node):
    if g.noVertex==1:
        canLab=findCanonical(g)
        if canLab in dagMap:
            root.children.add(dagMap[canLab])
        else:
            newNode = Node()
            dagMap[canLab]=newNode
            newNode.canLab=canLab
            root.children.add(dagMap[canLab])
        return
    for i in g.vlabels:
        g1=copy.deepcopy(g)
        g1.id='#'
        g1.noVertex-=1
        del(g1.vlabels[i])
        for tup in g1.adj[i]:
            del(g1.edges[(i,tup[0])])
        del(g1.adj[i])
        for v in g1.adj:
            for tup in g1.adj[v]:
                if tup[0]==i:
                    g1.adj[v].remove(tup)
                    del(g1.edges[(v,tup[0])])
        if not isConnected(g1):
            continue
        else:
            canLab=findCanonical(g1)
            if canLab in dagMap:
                dagMap[canLab].children.add(node)
            else:
                newNode = Node()
                newNode.canLab=canLab
                dagMap[canLab]=newNode
                dagMap[canLab].children.add(node)
                insertSubgraphs(g1,newNode)

def createIndex(inputfile):
    global count
    createGraphs(inputfile)
    #printGraphs()
    print("Graphs created")
    for g in graphs:
        canLab=findCanonical(graphs[g])
        #print(canLab)
        if canLab in dagMap:
            dagMap[canLab].gid.append(graphs[g].id)
        else:
            newNode = Node()
            newNode.gid.append(graphs[g].id)
            newNode.canLab=canLab
            dagMap[canLab]=newNode
            insertSubgraphs(graphs[g],newNode)
    storeIndex()
    print("Index structure stored")


if __name__ == "__main__":
    inputfile=sys.argv[1]
    createIndex(inputfile)

    tempfile=open("inputfile_BHK.txt",'w')
    tempfile.write(inputfile)
    tempfile.close()


