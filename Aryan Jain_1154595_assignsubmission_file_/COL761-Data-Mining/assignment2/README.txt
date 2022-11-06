i) 
root :
    - binaries : contains the binaries for fsg, gaston and gspan as provided by the assignment
                instructions
        - fsg
        - gaston
        - gspan
    
    - q1 : contains the code for q1
        - transformInput.py : python script for transforming the input dataset to 
                                appropriate formats for fsg, gspan and gaston respectively
        - plot.py : python script to plot the graph comparing the runtimes for the different
                    methods on all support threshold values
        - measure_times.sh : script for using the aforementioned python scripts to run gspan, fsg
                            and gaston on the given dataset and then plot the graph
    
    - q2 : contains the source code for q2
        - include
            - graph-utils.hpp
            - LibGastonForSofia.h : header file for gaston for sofia library 
                                    [https://github.com/AlekseyBuzmakov/LibGastonForSofia]
        - lib
            - libGaston.so : binary for gaston for sofia
        - src
            - convertTogspan.cpp : converts the dataset to appropriate format for gspan
            - graph-utils.cpp : util functions for q2
            - index.cpp : code for creating and saving an index based on frequent substructures 
                        found by gaston
            - query.cpp : runs the queries given through the input file
            - runGaston.cpp : runs libGaston.so on the input file and generates substructures

    - q3 : 
        - k_means.py : python script to run k_means, plot the elbow plot and also find the best 
                        fitting value of "k" given the elbow plot.

    - compile.sh : compiles the code for q2
    - elbow_plot.sh : runs the python script for q3 and generates the elbow plot
    - index.sh : runs the indexing code and generates the index
    - query.sh : runs the querying code and generates the querying outputs


ii) 

Aryan Jain - 2019CS10334
Ramneet Singh - 2019CS50445
Anjali Sharma - 2019CS50422

iii)

run `source CS1190334_install.sh` to clone the repository and cd into it and load modules.

**YOU NEED TO CHANGE THE PERMISSION TO 775 FOR THE SCRIPTS**

For Question 1.

Run the script measure_times.sh to run gspan, gaston and fsg on the given dataset 
and to plot the graph comparing the runtimes for the different methods on all support
threshold values:
    `sh measure_time.sh <dataset>`

For Question 2.

The list of IDs in the output of the queries are not sorted on any specific basis

1. To compile our code:
    `sh compile.sh`

2. To make the index:
    `sh index.sh <dataset>`

3. To run queries:
    `sh query.sh`
    enter the name of the file containing the query graphs when prompted


For Question 3.

To make the elbow plot:
    `sh elbow_plot.sh <dataset> <dimensions> q3_<dimensions>_<RollNo>.png`

iv)

Aryan Jain : 33%
Ramneet Singh : 33%
Anjali Sharma : 33%