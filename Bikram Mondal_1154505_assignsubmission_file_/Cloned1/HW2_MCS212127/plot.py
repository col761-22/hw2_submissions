import time
import subprocess
import matplotlib.pyplot as plt
import numpy as np

#ts=[99]
ts=[5,10,25,50,95]
res=[]
cmds=[
    './gSpan-64 -f correct_gaston -o -i -s ',
    './gaston ',
    './fsg correct_gaston -s '
]
for i in range(3):
    var=[]
    for t in ts:
        c=cmds[i]
        if i==0:
            t/=100
            c+=str(t)
        elif i==1:
            c+=str(t)+' correct_gaston -p'
        else:
            c+=str(t)
        start = time.time()
        print(c)
        subprocess.run(c.split(" "))
        end = time.time()
        var.append(round(end - start, 3))
    res.append(var)
print(res)
barWidth = 0.25
fig = plt.subplots(figsize =(12, 8))


br1 = np.arange(len(res[0]))
br2 = [x + barWidth for x in br1]
br3 = [x + barWidth for x in br2]
 
# Make the plot
plt.bar(br1, res[0], color ='r', width = barWidth,
        edgecolor ='grey', label ='gSpan')
plt.bar(br2, res[1], color ='g', width = barWidth,
        edgecolor ='grey', label ='gaston')
plt.bar(br3, res[2], color ='b', width = barWidth,
        edgecolor ='grey', label ='fsg')
 
# Adding Xticks
plt.title('Comparison between graph mining algorithms')
plt.xlabel('minSupport (%)', fontweight ='bold', fontsize = 15)
plt.ylabel('Time (secs)', fontweight ='bold', fontsize = 15)
plt.xticks([r + barWidth for r in range(len(res[0]))],
        [str(i) for i in ts])
 
plt.legend()
# plt.show()
plt.savefig("graphQ1.png")
