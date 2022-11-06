from matplotlib import pyplot as plt
import os
import time
from collections import defaultdict
from config import Config
from datetime import datetime

# Path to the input dataset
input_dataset = "167.txt_graph"

def read_dataset(filename):
	"""Helper function to read file from disk"""
	with open(filename) as fo:
		return fo.readlines()

def node_process(method_name:str, num_vertex_edge:int, line, node_lookup_str_value=None) -> str:
	"""Parses the line for NODE as required by different algorithms"""
	if method_name == 'fsg':
		line_vertex = f"v {num_vertex_edge} {line}\n"
	elif method_name in ['gaston', 'gspan']:
		line_vertex = f"v {num_vertex_edge} {node_lookup_str_value[line]}\n"
	
	return line_vertex

## 3 Python lists to record execution time of each algorithm at different support threshold levels
output_list_gspan = []
output_list_fsg = []
output_list_gaston = []

def pretty_print(message):
	print_val = f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}::\t{message}"
	print(print_val)

def edge_process(method_name:str,line) -> str:
	"""Parses the line for EDGE as required by different algorithms"""
	if method_name == 'fsg':
		line_edge = f"u {line}\n"
	elif method_name in ['gspan', 'gaston']:
		line_edge = f"e {line}\n"
	return line_edge


def execution_cmd_gaston(support_threshold, S, algo_filename) -> str:
	return f"./gaston-1.1-re/gaston {S} {algo_filename} {algo_filename}_output_{support_threshold}.txt"

## Reading values from config
algorithm_list = Config.algorithm_list
support_thresholds = Config.support_thresholds
number_of_graphs = Config.number_of_graphs
zero_val = Config.zero_val
generate_individual_plots = Config.generate_individual_plots
generate_cummulative_plot = Config.generate_cummulative_plot

def execution_cmd_gspan(support_threshold, algo_filename) -> str:
	return f"./gSpan61/gSpan -s {support_threshold/100.0} -f {algo_filename} -o"

def execution_cmd_fsg(support_threshold, algo_filename) -> str:
	return f"./pafi-1.0.1/Linux/fsg -s {support_threshold} {algo_filename}"


# Trimming the name of the input dataset file
dataset_name_wo_ext = input_dataset.split('.')[0]

## Converting the given datasets
for algorithm in algorithm_list:
	dataset_name_wo_ext = input_dataset.split('.')[0]
	output_name = algo_filename = dataset_name_wo_ext + '_' + algorithm + '.txt'


	## Reading the input datatset
	pretty_print("Reading input dataset")
	file_lines = read_dataset(input_dataset)

	with open(output_name,'w') as output_obj:
		pretty_print(f"Converting file for {algorithm}")
		
		if algorithm in ['fsg']:
			is_edge = True
			is_node = False
			num_vertex_edge = zero_val
			for i, line in enumerate(file_lines):
				if line == '\n':
					continue
				# Remove \n from line, NOTE: To be done after checking for empty line
				line = line.replace('\n','')

				## Graph ID starts with '#'. For every graph, start line with 't' in output file
				if line[0]=='#':
					output_obj.write('t\n')
					is_edge = True
					is_node = False
					num_vertex_edge = zero_val
				
				## A digit/number in input corresponds to either # of edges or # of nodes
				elif line.isdigit()==True:
					## For every new graph, we first have nodes in input file, followed by edges., 
					## Hence flipping the flags accordingly
					is_edge = not is_edge
					is_node = not is_node
					num_vertex_edge = zero_val
					
				## A new node has to be added to the output file in the format : v {id} {label}
				elif is_node:
					# line_vertex = f"v {num_vertex_edge} {line}\n"
					line_vertex = node_process(algorithm, num_vertex_edge, line)
					num_vertex_edge += 1
					output_obj.write(line_vertex)
				
				## A new edge has to be added to the output file in the format : u {source} {destination} {edge_label}
				elif is_edge:
					# line_edge = f"u {line}\n"
					line_edge = edge_process(algorithm, line)
					output_obj.write(line_edge)


		elif algorithm in ['gspan', 'gaston']:

			## Init variables
			is_edge = True
			is_node = False
			num_vertex_edge = zero_val
			transaction_number = zero_val
			node_lookup_str_value = defaultdict(lambda: None) #dict()
			vertex_count = zero_val
			
			for line in file_lines:
				if line == '\n':
					## Any empty line has to be skipped
					continue
				## Remove \n from line, NOTE: To be done after checking for empty line
				line = line.replace('\n','')
				
				# Graph ID starts with '#'. For every graph, start line with 't' in output file
				if line[0]=='#':
					output_obj.write('t # ' + str(transaction_number) + '\n')
					is_edge = True
					is_node = False
					num_vertex_edge = zero_val
					transaction_number += 1
				elif line.isdigit()==True:
					is_edge = not is_edge
					is_node = not is_node
					num_vertex_edge = zero_val
				elif is_node: # is_edge==0:
					# Update the lookup dictionary
					if line not in node_lookup_str_value:
						node_lookup_str_value[line] = vertex_count
						vertex_count = vertex_count+1
					
					# line_vertex = f"v {num_vertex_edge} {node_lookup_str_value[line]}\n"
					line_vertex = node_process(algorithm, num_vertex_edge, line, node_lookup_str_value)
					num_vertex_edge += 1
					output_obj.write(line_vertex)
				elif is_edge:
					# line_edge = f"e {line}\n"
					line_edge = edge_process(algorithm, line)
					output_obj.write(line_edge)


## Running the programs on all the methods and storing the times
for algorithm in algorithm_list:
	algo_filename = dataset_name_wo_ext + '_' + algorithm + '.txt'

	for i, support_threshold in enumerate(support_thresholds):
		pretty_print(f">>> METHOD: {algorithm}\t>>> Minsup: {support_threshold}")
		if algorithm=='gspan':
			command_str = execution_cmd_gspan(support_threshold, algo_filename)
		elif algorithm=='fsg':
			command_str = execution_cmd_fsg(support_threshold, algo_filename)
		elif algorithm=='gaston':
			S = (support_threshold * number_of_graphs)/100.0
			command_str = execution_cmd_gaston(support_threshold, S, algo_filename)
		
		start_time = time.time()
		os.system(command_str)
		end_time = time.time()

		if algorithm == 'gspan':
			output_list_gspan.append(end_time-start_time)
			print(support_threshold, end_time-start_time)

		elif algorithm == 'fsg':
			output_list_fsg.append(end_time-start_time)
			print(support_threshold, end_time-start_time)

		else:
			output_list_gaston.append(end_time-start_time)
			print(support_threshold, end_time-start_time)
	
	if generate_individual_plots:
		pretty_print(f"Making plot for {algorithm}")
		plt.figure()

		if algorithm == 'gspan':	
			plt.plot(support_thresholds, output_list_gspan)
			plt.xlabel('Support Threshold %')
			plt.ylabel('Execution time (s)')
			plt.title('Execution time v/s suport threshold for: ' + algorithm)
			plt.savefig(f'running_time_plot_{algorithm}.png')
			
		elif algorithm == 'fsg':
			plt.plot(support_thresholds, output_list_fsg)
			plt.xlabel('Support Threshold %')
			plt.ylabel('Execution time (s)')
			plt.title('Execution time v/s suport threshold for: ' + algorithm)
			plt.savefig(f'running_time_plot_{algorithm}.png')
		else:
			plt.plot(support_thresholds, output_list_gaston)
			plt.xlabel('Support Threshold %')
			plt.ylabel('Execution time (s)')
			plt.title('Execution time v/s suport threshold for: ' + algorithm)
			plt.savefig(f'running_time_plot_{algorithm}.png')
	
if generate_cummulative_plot:
	pretty_print("Making cummulative plot...")
	plt.figure()
	plt.plot(support_thresholds, output_list_fsg, label='FSG')
	plt.plot(support_thresholds, output_list_gaston, label='Gaston')
	plt.plot(support_thresholds, output_list_gspan, label='gSpan')
	plt.xlabel('Support Threshold %')
	plt.ylabel('Execution Time (s)')
	plt.title('Execution time v/s suport threshold')
	plt.legend()
	plt.savefig('running_time_plot.png')

pretty_print("---- DONE ----")