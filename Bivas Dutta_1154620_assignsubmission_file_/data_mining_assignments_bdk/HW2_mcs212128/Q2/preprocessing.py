
def main(f,ooo):
    fi=open(f)
    op=open(ooo,'w')
    # op2=open('graph_label','w')
    c=k=0
    vertex=0
    dic={}
    import sys
    while True:
        f=fi.readline()
        if not f:
            break
        if f[0]=='#':
            op.write('t '+'# '+ f[1:])
            # op2.write(str(c)+' '+f[1:])
            vertex=int(fi.readline())
            i=0
            while i<vertex:
                label=fi.readline()
                # print(label)
                # if label:
                label=label.strip()
                if label not in dic:
                    dic[label]=k
                    k+=1
                # print(label)
                val=0
                for j in label:
                    val+=(ord(j))
                op.write('v '+str(i)+' '+ str(val)+'\n')
                i+=1
            edges=int(fi.readline())
            i=0
            eg=[]
            while i<edges:
                edge=fi.readline()
                edge=edge.strip().split()


                eg.append((int(edge[0]),int(edge[1]),int(edge[2])))
                # op.write('e '+edge)
                i+=1
            eg.sort()
            
            # op.open(ooo,'w')

            for i,j,k in eg:
                # if sys.argv[1]=='fsg':
                #     op.write('u '+str(i)+' '+str(j)+' '+str(k)+'\n')
                # else:
                op.write('e '+str(i)+' '+str(j)+' '+str(k)+'\n')


            c+=1
        
        
