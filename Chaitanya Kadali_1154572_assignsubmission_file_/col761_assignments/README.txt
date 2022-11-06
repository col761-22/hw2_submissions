GitHub Repo Link : https://github.com/cs5190435/col761_assignments

i) Files i included are
q1.cpp  			--> This file reads the given database and performs gSpan, fsg, gaston on database.
q1.py 				--> Used for plotting the graph at various percentages..
q1.sh <graph_database>  	--> This will run the q1.cpp on the graph_database and run q1.py to generate q1_CS5190435.png

index.cpp 			--> To produce all the index structures of the given database.
help.cpp 			--> This will ask for query file and then produce the desired output
index.sh <graph_database>	--> This will run index.cpp on the given graph_database and produces index structures in Dump/
query.sh 			--> This will first load all the index structures and ask for query. 

compile.sh 			--> This will produce the executables for help.cpp and index.cpp

kmeans.py 			--> This is the python file required to run 3rd part
elbow_plot.sh 			--> This will execute python file kmeans.py <dataset> dimension

Other folders are the executables of gaston,gspan,fsg.

ii) Team Members:
1. Chaitanya Naga Sai Kadali 	- 2019CS50435	-- 33.3%
2. Roshan Raj Karingla 		- 2019CS50437	-- 33.3%
3. Shubankar Sarkar 		- 2022AIZ8171	-- 33.3%


iii) Instructions on how to execute the code

To execute q1:
sh q1.sh <graph_dataset>

To execute q2:
sh compile.sh
sh index.sh <graph_dataset>
sh query.sh

Then, there will be a interactive command which will ask for query file. After provided, it will output to output_CS5190435.txt

To execute q3:
sh elbow_plot.sh <data_set> dimension




