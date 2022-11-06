import sys
import time
import subprocess
import matplotlib.pyplot as plt


thresholds = [5, 10, 25, 50, 95]  # x axis


main_list = []

commands = ["./fsg output_graph_fsg -s ",
            "./gSpan -f output_graph_2 -s ",
            "./gaston "
]

for i,c in enumerate(commands):
    var = []
    for t in thresholds:
        cmd = c
        if i == 1:
            t /=100
        if i == 2:
            cmd+=str(t)+" output_graph_2 -p"
        else:
            cmd+=str(t)
        start = time.time()
        print(cmd)
        subprocess.run(cmd.split(" "))
        end = time.time()
        var.append(round(end - start, 3))
    main_list.append(var)


print(main_list)
plt.xlabel('Threshold')
plt.ylabel('Time(M. sec)')
plt.title('Time Vs. Threshold Graph')
plt.plot(thresholds,main_list[0], c='g')
# plt.legend(['gSpan'])
plt.plot(thresholds,main_list[1], c='b')
# plt.legend(['fsg'])
plt.plot(thresholds,main_list[2], c='r')
plt.legend(['gSpan','fsg','gaston'])

plt.savefig("finding_graph_patterns.png")


