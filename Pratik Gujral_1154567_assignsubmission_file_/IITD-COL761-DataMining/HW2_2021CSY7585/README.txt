*******IMPORTANT NOTE*********
FOR Q1:
-------
1) pip install -r requirements.txt
2) python3 q1_code.py

Explanation of files:
- requirements.txt --> Contains the versions of libraries to recreate the environment for code to run
- q1_code.py --> Code file for question 1



FOR Q2:
-------
Note: instructions to run code given in point number 3.

1) EXPLANATION OF THE FILES

--- Makefile, sample_query, sample_dataset need not be used, they were helper tools for us

--- gSpan folder contains the gSpan binary

--- index.sh - takes as input the dataset (.txt) and compiles and runs the two cpp files i.e. conv_input.cpp and
helper.cpp with support as 50

--- conv_input.cpp - the convToGspan() function converts the original dataset to a format that can be taken as input by the "gSpan" binary. The main function runs a system line command that runs gSpan on the converted dataset to output in a file called "output.txt.fp"

--- helper.cpp - This is our main file which builds the index and runs the queries!!! We explain the functions one by one:

0) maxL -> maximum number of edges that can be in a feature graph; gammaThreshold -> threshold value of gamma function (explained below); M -> maximum number of features allowed

i) We use the boost/graph utility for graph building, iteration and lastly boost/vf2 for isomorphism checking

ii) The typdefinitions are just property maps that map features of one graph to another

iii) bool isomorphismchecker() - check isomorphism from a grpah to its subgraph and returns a corresponding boolena value

iv) bool checker() - utility function just to check whether a string only contains a number or may contain something else --> helps in reading input from dataset

v) void setIntersection() - takes out the intersection between two sets and stores it in the first argument

vi) void readDataset() - reads the dataset of graphs and stores is in a vector of graphs passed as argument, the graph here is from the boost library, we also store here the tags/ids of the graphs in a map

vii) void readGspanOutput() - reads the frequent graphs from gSpan output into the form of boost graphs and stores them in a vector, the format in which they are stored (both ids and graphs) is that they are stored as a number of edges in graph to the corresponding graph. This will help us in building features later.

viii) void pushAllFeatures() - utility function to help push a single edge, converting that edge to a graph and storing it in 1-edge features in the variable "all_features"

ix) void pustToGraphMap() - pushes all mappings of 1-edge graphs to the set of graph ids that they are mapped to in the "graph_mp" variable

x) double findGamma() - this function returns the gamma value of a graph in the gSpan output. What is gamma value? --> It is the ratio of (Number of graphs in the intersection set of all the features contained in a graph x)/(Number of graphs containing x as a feature)
Use? --> If the ratio is close to 1 then we can say that the feature x is redundant and shouldn't be used in our index
Reference paper - https://sites.cs.ucsb.edu/~xyan/papers/sigmod04_gindex.pdf

xi) void eliminateRedundantGraphs() - after index building this feature removes the redundant graphs that need not be checked for isomorphism because they did not have the features the query graph has!

xii) void fillQueries() - fills a vector with the query graphs from the query file

xiii) int main() - the main function reads the dataset, reads the frequent subgraphs from gSpan, builds single-edged features from the read dataset and then further builds upon to multiple edges features using the gamma function. After indexing is done queries are read and stored in the output file.

2) TEAM MEMBERS:

SUYASH SINGH - 2018CS50646
PRATIK - 2021CSY7585
YASH NARENDRA - 2021ANZ8639

3) INSTRUCTIONS TO RUN CODE:

use sh index.sh <dataset.txt> to run code

******IMPORTANT****** --> for the interactive command line input for query file if the prompt to enter the query name does not appear please enter the file name after sometime!!!

4) CONTRIBUTION OF EACH STUDENT

Each team member contributed equally whether it be thinking about how to proceed with ideas, making plots, helper functions and main utility functions. All of read through internet and other sources on how to build the index, understood how to use the vf2 library and then wrote the code:

SUYASH - 33%
PRATIK - 33%
YASH - 33%
