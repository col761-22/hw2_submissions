import matplotlib.pyplot as plt
import subprocess
import sys
from timeit import timeit
import math
import os 
import json
dir_path = os.path.dirname(os.path.realpath(__file__))
print(dir_path)
print("directory path", dir_path)
infile = sys.argv[1] #dir_path +"/Yeast/167.txt_graph" #
outfile = sys.argv[2] #"image.png" #
support_thresholds = [5,20,25,50,90,95] 

fsg_times = []
gspan_times = []
gaston_times = []


def get_data_in_fsg_format(infile,outfile):
    fin = open(infile,"r")
    fout = open(outfile,"w")
    EOF = False
    item = fin.readline()
    while not EOF:
        if item[0] == '#':
            fout.write("t " + item.strip() + "\n")
            num_nodes = int(fin.readline().split()[0])
            for i in range(num_nodes):
                fout.write("v" + " " + str(i) + " "+  fin.readline().split()[0] + "\n")
            num_edges = int(fin.readline().split()[0])
            for i in range(num_edges):
                fout.write("u" + " " + " ".join(fin.readline().split()) + "\n")
        item = fin.readline()
        if not item:
            fin.close()
            fout.close()
            return

def get_data_in_gspan_format(infile,outfile,label_dict_file):
    fin = open(infile,"r")
    fout = open(outfile,"w")
    EOF = False
    item = fin.readline()
    label_dict = {}
    num_graphs = 0
    ct = 0
    while not EOF:
        if item[0] == '#':
            num_graphs += 1
            fout.write("t"+ item.strip() + "\n")
            num_nodes = int(fin.readline().split()[0])
            for i in range(num_nodes):
                label = fin.readline().split()[0]
                if label in label_dict:
                    label  = label_dict[label]
                else:
                    label_dict[label] =ct
                    label = ct
                    ct += 1
                fout.write("v" + " " + str(i) + " "+ str(label)  + "\n")
            num_edges = int(fin.readline().split()[0])
            for i in range(num_edges):
                line = fin.readline().split()
                start_node = line[0]
                end_node = line[1]
                label = line[2]
                if label in label_dict:
                    label  = label_dict[label]
                else:
                    label_dict[label] =ct
                    label = ct
                    ct += 1
                fout.write("e" + " " + " ".join([start_node,end_node,str(label)]) + "\n")
        item = fin.readline()
        if not item:
            json.dump(label_dict,open(label_dict_file,"w"))
            fin.close()
            fout.close()
            return num_graphs

def get_data_in_gaston_format(infile,outfile,label_dict_file):
    fin = open(infile,"r")
    fout = open(outfile,"w")
    EOF = False
    item = fin.readline()
    label_dict = {}
    num_graphs = 0
    ct = 0
    while not EOF:
        if item[0] == '#':
            num_graphs += 1
            fout.write("t"+ item.strip() + "\n")
            num_nodes = int(fin.readline().split()[0])
            for i in range(num_nodes):
                label = fin.readline().split()[0]
                if label in label_dict:
                    label  = label_dict[label]
                else:
                    label_dict[label] =ct
                    label = ct
                    ct += 1
                fout.write("v" + " " + str(i) + " "+ str(label)  + "\n")
            num_edges = int(fin.readline().split()[0])
            for i in range(num_edges):
                line = fin.readline().split()
                start_node = line[0]
                end_node = line[1]
                label = line[2]
                if label in label_dict:
                    label  = label_dict[label]
                else:
                    label_dict[label] =ct
                    label = ct
                    ct += 1
                fout.write("e" + " " + " ".join([start_node,end_node,str(label)]) + "\n")
        item = fin.readline()
        if not item:
            json.dump(label_dict,open(label_dict_file,"w"))
            fin.close()
            fout.close()
            return num_graphs

def run_system_call_fsg(code_type,support,filename):
    if code_type =="fsg":
        subprocess.run([dir_path+"/binaries/pafi-1.0.1/Linux/fsg","-s",str(support), filename])

def run_system_call_gspan(code_type,support,filename):
    if code_type == "gspan":
        subprocess.run([dir_path+"/binaries/gSpan6/gSpan","-f",filename,"-s",str(support)])

def run_system_call_gaston(code_type,support,filename):
    if code_type == "gaston":
        subprocess.run([dir_path+"/binaries/gaston-1.1/gaston",str(support),filename])


def measure_time_function(func, *args, **kwargs):
    def time_function():
        return func(*args, **kwargs)
    return time_function

### FSG time ###
print("capturing fsg")
get_data_in_fsg_format(infile,"temp_file_fsg")
for support in support_thresholds:
    time_function = measure_time_function(run_system_call_fsg, "fsg",support,"temp_file_fsg")
    fsg_times.append(timeit(time_function, number=1))
### GSPAN time ###
print("capturing gspan")
num_graphs = get_data_in_gspan_format(infile,"temp_file_gspan","label_dict")
for support in support_thresholds:
    time_function = measure_time_function(run_system_call_gspan, "gspan",(support*1.0/100),"temp_file_gspan")
    gspan_times.append(timeit(time_function, number=1))

### GASTON TIME ###
print("capturing gaston")
num_graphs = get_data_in_gaston_format(infile,"temp_file_gaston","label_dict")
for support in support_thresholds:
    time_function = measure_time_function(run_system_call_gaston, "gaston",math.ceil(support*num_graphs*1.00/100),"temp_file_gaston")
    gaston_times.append(timeit(time_function, number=1))

plt.plot(support_thresholds,fsg_times,'-b',label = '-FSG')
plt.plot(support_thresholds,gspan_times,'-r',label = '-GSPAN')
plt.plot(support_thresholds,gaston_times,'-g',label = '-GASTON')
plt.xlabel('Support thresholds %')
plt.ylabel('Running time (seconds)')
plt.legend(loc='upper right')
plt.title('Comparision of thresholds vs running time for fsg,gspan,gaston')
plt.savefig(outfile)
