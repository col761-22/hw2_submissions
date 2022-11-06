import sys
import os
import subprocess
import preprocessing
import time
sys.setrecursionlimit(10**9)
import tracemalloc
#you can make a struct
class TreeNode:
    def __init__(self,key) -> None:
        self.key=key
        self.next=[]
        pass
has=2048
import copy
#graphs=> dic, key=>graph_id,value=> adj list of corresponding graph id,hash map of vector of string
graphs={}
#queries=> key=> query id, value=> adj of query
queries={}
#hash_graph=dic, key=>hash value of canonical label of graph<int>, value=> graph id<string>
hash_graph={}
#all_tree_nodes=dic, key=> hash value of canonical label<int>,value=> adress<pointer I think>
all_tree_nodes={}
#canonical dic, key=>canonical label<string> value=>hash value<int>
canonical={}
#op_nodes => all output graph ids vector<strings>
op_nodes=set()
# all_ele dic key=hash value <int> value= list of ids vector <string> 
all_ele={}

def find(graph,i,j):
    for vid,edlbl in graph[i][1]:
        if vid==j:
            return edlbl
    return ''

def cano(graph):
    if not graph:
        return ''
    dic={}
    alll=[]
    for uid in graph:
        t=[]
        #t is vector<vector<str,str,str>>
        # print(graph[uid])
        # for ulabel,adjlist in graph[uid]:
        for vid,elabel in graph[uid][1]:
                t.append((graph[vid][0],elabel,str(len(graph[vid][1]))))
        t.sort()
        t=tuple(t)
        alll.append(t)
        if t not in dic:
            dic[t]=[uid]
        else:
            dic[t].append(uid)
    # print(dic,sorted(alll))
    res_ord=[]
    #res_ord=vector<string>
    for i in sorted(alll):
        if i in dic:
            res_ord.append(dic[i].pop())
    n=len(res_ord)
    # print(res_ord)
    c=''
    for i in res_ord:
        c+=graph[i][0]
    # c+=''.join(res_ord)
    # mat=[[0 for _ in range(n)] for _ in range(n)]
    for i in range(n):
        for j in range(i,n):
            if i!=j:
                edlbl=find(graph,res_ord[i],res_ord[j])
                # if edlbl:
                c+=edlbl
    # print(c)

    return c

def find_node(c):
    # print(c)
    if c in hash_graph:
        # print(canonical[c])
        # print('yeah')
        for i in all_ele[c]:
            op_nodes.add(i)
        # # print(op_nodes)
        # if c in all_tree_nodes:
        #     children=all_tree_nodes[c]
        #     # print(children.next.key)
        #     for child in children.next:
        #         find_node(child.key)
    else:
        children=all_tree_nodes[c]
        # print(children.next.key)
        for child in children.next:
            find_node(child.key)

def search(graph):
    # print("bal")
    c=canonical_label_single(graph)
    # print(c)
    if c in all_tree_nodes:
        find_node(c)
    # else:
    #     print('fuvk')
    
def dfs(graph,vis,u):
    vis[u]=1
    for v,_ in graph[u][1]:
        if v not in vis:
            dfs(graph,vis,v)

def del_vertex(vartex,adj):
    new_gr={}
    #new_gr is same as adj
    vis={}
    # vis is dic key=id <str> if id in dic => present else absent
    for i in adj:
        if i!=vartex:
            new_gr[i]=[adj[i][0],[]]
            # vis[i]=0
            for adj_ver,e_label in adj[i][1]:
                if adj_ver!=vartex:
                    new_gr[i][1].append([adj_ver,e_label])
    c=0
    for i in new_gr:
        if i not in vis:
            c+=1
            if c>1:
                return 
            dfs(new_gr,vis,i)
            
    # if c==1 or c==0: 
    return new_gr
    # print(vartex,"bal-bara",c)
    # return {}
    # neww={}
    # for i in new_gr:
    #     if new_gr[i][1]:
    #         neww[i]=[new_gr[i][0],new_gr[i][1]]
    # return neww

    pass

def buildtree_util(adj_graph,parent_add):
    # if len(adj_graph)>=20:
    # if len(adj_graph)==0:
    #     exit()
    # print(len(adj_graph))
    if not adj_graph:
        return
    # print(len(adj_graph))
    for vartex in adj_graph:
        new_graph=del_vertex(vartex,adj_graph)
        if new_graph:
            c_label=canonical_label_single(new_graph)
            if c_label not in all_tree_nodes:
                all_tree_nodes[c_label]=TreeNode(c_label)
                all_tree_nodes[c_label].next.append(parent_add)
                buildtree_util(new_graph,all_tree_nodes[c_label])
            else:
                all_tree_nodes[c_label].next.append(parent_add)
        del new_graph

def buildTree(all_tree_nodes):
    i=0
    xx=copy.deepcopy(all_tree_nodes)
    for hash_value in xx:
        address=all_tree_nodes[hash_value]
        graph_id=hash_graph[hash_value]
        adj_graph=graphs[graph_id]
        for vartex in adj_graph:
            new_graph=del_vertex(vartex,adj_graph)
            c_label=canonical_label_single(new_graph)
            if c_label not in all_tree_nodes:
                # print(c_label)
                all_tree_nodes[c_label]=TreeNode(c_label)
                all_tree_nodes[c_label].next.append(address)
                buildtree_util(new_graph,all_tree_nodes[c_label])
            else:
                all_tree_nodes[c_label].next.append(address)
            del new_graph
        print("done",i)
        i+=1

def canonical_label_single(adj):
    c=cano(adj)
    s=0
    for i in c:
        s+=ord(i)
    return s%has
    
    s=''
    for i in adj:
        ad=adj[i]
        s+=ad[0]
        s+=str(len(ad[1]))
    s=list(s)
    s.sort()
    s=(''.join(s))
    canonical[s]=hash(s)
    return hash(s)
    
def import_graphs_to_tree(hash_graph):
    for c_label in hash_graph:
        adress=TreeNode(c_label)
        all_tree_nodes[c_label]=adress

def canonical_label(graphs):
    for graph in graphs:
        # print(graph)
        adj=graphs[graph]
        s=cano(adj)
        # for i in adj:
        #     ad=adj[i]
        #     s+=ad[0]
        #     s+=str(len(ad[1]))
        #     s=list(s)
        #     s.sort()
        #     s=(''.join(s))
        # canonical[s]=hash(s)
        c=0
        for i in s:
            c+=ord(i)
        # c=hash(s)
        canonical[s]=c%has
        hash_graph[c%has]=graph
        if c%has not in all_ele:
            all_ele[c%has]=[graph]
        else:
            all_ele[c%has].append(graph)

def build_graph(graph):
    id=graph[0].strip().split()[2]
    adj={}
    #adj: key=>vid, value=[a,b] where a=v_label and b=list of adjacent vertices,b=[[c,d]] c=>adjacent vertex,d=edge label;
    for i in graph[1:]:
        i=i.strip().split()
        if i[0]=='v':
            adj[i[1]]=[i[2],[]]
        if i[0]=='e':
            #i[1]=id1,i[2]=id2,i[3]=edge label
            adj[i[1]][1].append([i[2],i[3]])
            adj[i[2]][1].append([i[1],i[3]])
    # print(adj)
    graphs[id]=adj

def build_query(graph):
    # print(graph)
    if not graph:
        return 
    # def build_graph(graph):
    id=graph[0].strip().split()[2]
    adj={}
    #adj: key=>vid, value=[a,b] where a=v_label and b=list of adjacent vertices,b=[[c,d]] c=>adjacent vertex,d=edge label;
    for i in graph[1:]:
        i=i.strip().split()
        if i[0]=='v':
            adj[i[1]]=[i[2],[]]
        if i[0]=='e':
            #i[1]=id1,i[2]=id2,i[3]=edge label
            adj[i[1]][1].append([i[2],i[3]])
            adj[i[2]][1].append([i[1],i[3]])
    # print(adj)
    queries[id]=adj
    # return adj

def readfile(f):
    fi=open(f)
    graph=[]
    while True:
        # print(graph)
        f=fi.readline()
        if not f:
            build_graph(graph)
            break
        if f[0]=='t':
            graph.append(f)
            while True:
                x=fi.readline()
                # print(x)
                if not x:
                    break
                if x[0]=='t':
                    # print(graph)
                    build_graph(graph)
                    graph=[x]
                    continue
                graph.append(x)

def queryread(filll):
    fi=open(filll)
    query=[]
    while True:
        # print(graph)
        f=fi.readline()
        if not f:
            build_query(query)
            break
        if f[0]=='t':
            query.append(f)
            while True:
                x=fi.readline()
                # print(x)
                if not x:
                    break
                if x[0]=='t':
                    # print(graph)
                    build_query(query)
                    query=[x]
                    continue
                query.append(x)

def save_queries(i):
    o=subprocess.run('rm -r ./queries/*',shell=True)
    # print(o.stderr)
    c=0
    # print(op_nodes)
    # for i in queries:
    adj=queries[i]
    op=open('./queries/q'+str(i)+'.sub'+'.grf','w')
    c+=1
    op.write(str(len(adj))+'\n')
    for id in adj:
        orr=0
        for i in adj[id][0]:
            orr+=ord(i)
        op.write(str(id)+' '+str(orr)+'\n')
    for i in range(len(adj)):
        i=str(i)
        op.write(str(len(adj[i][1]))+'\n')
        if not adj[i][1]:
            op.write(str(0))
        else:
            for j,ed in adj[i][1]:
                op.write(str(i)+' '+str(j)+" "+str(ed)+'\n')

def save_res(op_nodes):
    o=subprocess.run('rm -r ./res/*',shell=True)
    # print(o.stderr)
    c=0
    # print(op_nodes)
    for i in op_nodes:
        adj=graphs[i]
        op=open('./res/res'+str(i)+'.grf','w')
        c+=1
        op.write(str(len(adj))+'\n')
        for id in adj:
            orr=0
            for i in adj[id][0]:
                orr+=ord(i)
            op.write(str(id)+' '+str(orr)+'\n')
        for i in range(len(adj)):
            i=str(i)
            op.write(str(len(adj[i][1]))+'\n')
            if not adj[i][1]:
                op.write(str(0))
            else:
                for j,ed in adj[i][1]:
                    op.write(str(i)+' '+str(j)+' '+str(ed)+'\n')

def runvf2():
    res=[]
    allgrfs=os.listdir('./res')
    allsubgrfs=os.listdir('./queries')
    print('finding for ',allsubgrfs)
    print(len(allgrfs))
    # for querie in queries:
        
    for dir in allgrfs:
        # print('running',dir)
        st=subprocess.Popen(['./a.out','./queries/'+allsubgrfs[0],'./res/'+dir],stdout=subprocess.PIPE)
        # st=subprocess.run('./a.out '+ './queries/'+allsubgrfs[0]+' '+ './res/'+dir,shell=True)
        stdout=st.communicate()[0]
        # print("python",str(stdout))
        if 'done' in str(stdout):
            res.append(dir.split('.')[0][3:])
        # if str(st.stdout).strip().split()[0][2:] !='0':
        #     res.append(dir.split('.')[0][3:])
    return res

def runvf3():
    res=[]
    allgrfs=os.listdir('./res')
    allsubgrfs=os.listdir('./queries')
    print('finding for ',allsubgrfs)
    print(len(allgrfs))
    # for querie in queries:
        
    for dir in allgrfs:
        # print('running',dir)
        st=subprocess.run('./vf3lib-master/bin/vf3 '+ './queries/'+allsubgrfs[0]+' '+ './res/'+dir,shell=True, capture_output=True)
        # print(str(st.stdout))
        if str(st.stdout).strip().split()[0][2:] !='0':
            res.append(dir.split('.')[0][3:])
    return res


if __name__=='__main__':
    # tracemalloc.start()
    print('Loading index...')
    f=open('input_graph')
    f=f.read().strip()
    # print(f)
    preprocessing.main(f,'op_1')
    # print("format changed for file")
    readfile('op_1')
    # print('reading')
    canonical_label(graphs)
    # print(graphs['310'])
    # print('sep')
    import_graphs_to_tree(hash_graph)
    buildTree(all_tree_nodes)
    # queryread()
    # print(canonical)
    print('Index loaded.')
    # preprocessing.main(sys.argv[2],'query')
    st=subprocess.Popen(['chmod', '+x','./a.out'],stdout=subprocess.PIPE)
    while True:
        print('enter a Query file, To break type "done"')
        qf=input()
        if qf=='done':
            break
        preprocessing.main(qf,'query')
        queryread('query')
        # print(queries)
        # save_queries()
        print('query save')
        print("searching")
        # c=0
        ts=time.time()
        for i in queries:
            op_nodes=set()
            querygraph=queries[i]
            st=time.time()
            search(querygraph)
            save_queries(i)
            save_res(op_nodes)
            end=time.time()
            print("op for query #",i,"the results are",runvf2(),'time Taken: ',str(round(end - st, 3)))
            # c+=1
        te=time.time()
        print('total time taken',round((te-ts),3))
    