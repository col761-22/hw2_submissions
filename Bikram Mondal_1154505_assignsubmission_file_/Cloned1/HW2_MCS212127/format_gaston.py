import sys
dt=sys.argv[1]
f=open(dt,"r")
w=open('correct_gaston',"w+")
weird = f.readline()
while weird != "\n":
    n=weird
    n=n.split('#')
    try:
        id=n[1]
    except:
        break
    w.write("t # "+id)
    n_v=int(f.readline())
    for i in range(n_v):
        l=f.readline()
        st=""
        asci=0
        for c in l[:-1]:
            asci+=ord(c)
            #st+=str(c)
        w.write("v "+str(i)+" "+str(asci)+"\n")
    n_e=int(f.readline())
    for i in range(n_e):
        l=f.readline()
        w.write("e "+l)
    bsl = f.readline()
    weird = f.readline()
# for i in f:
#     print(i)
f.close()
w.close()
