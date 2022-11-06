README for Assignment 2 : COL761 (Data Mining)
Prof. Sayan Ranu, Semester 1, 2022-23

Team:                              Contribution:-
Bikram Mondal(2021MCS2127)             33%
Harshit Verma(2021MCS2133)             33%
Kritika Gupta(2021MCS2136)             33%


Github Repository: https://github.com/bikram947/Data-Mining-COL761-.git


FILES PRESENT IN ZIP


FOR Q1:

    1. q1.sh: bash file to completely execute all three algorithms on thresholds (5,10,25,50,95)% on the Yeast dataset after converting it into desired 	     format for the algorithms and then plot the time taken by three graph mining algorithms against different thresholds.
    
    2. format_<algorithm name*>.py: Python code to preprocess the dataset into desired format for the particular algorithm. (here algorithm name*: fsg, gaston, 	 gSpan)
    
    3. 167.txt_graph: Yeast dataset
    
    4. plot.py: Python file to run commands for algorithms with different thresholds and then plot the time by threshold graph
    
    5. fsg, gaston, gSpan-64 binary files: Files required to execute respective algorithm

FOR Q2:

    1. include: Folder which contains boost library for subgraph isomorphism

    2. buildIndex.py: python file used to create index on given dataset

    3. search.py: python file to search the query from index and writes to output file

    4. vf2check.cpp: c++ file which uses vf2 code for subgraph isomosphism test

    5. index.sh: bash script which when executed creates index, and it compiles the vf2check.cpp.

    6. query.sh: bash script which when executed search query files graghs in index to find subgraph isomorphism. This will load index and then intractively ask 		for query file input

FOR Q3:

    1. kmeans.py: python file to run kmeans algorithm and to create the elbow plot.
    2. MCS212127_generated_dataset_2D.dat: Generated 2D dataset
    3. MCS212127_generated_dataset_3D.dat: Generated 3D dataset
    4. MCS212127_generated_dataset_4D.dat: Generated 4D dataset
    5. elbow_plot.sh: bash file which executes the kmeans.py
    6. q3_2_MCS21217.png: elbow plot for 2D dataset
    6. q3_4_MCS21217.png: elbow plot for 2D dataset
    7. q3_4_MCS21217.png: elbow plot for 2D dataset


STEPS TO EXECUTE THE CODES:

FOR Q1:
    
    Execute q1.sh by the command 'sh q1.sh <dataset_name>'
    
FOR Q2:

    Execute index.sh by the command 'sh index.sh <grapgh_dataset_name>'
    Execute query.sh by the command 'sh query.sh' and when ask for query file give queryfile name.
    (Note: We have not sorted the graph ids that we gat for each query.)
    
 FOR Q3:
    
    Execute elbow_plot.sh by the command 'sh elbow_plot.sh <dataset> <dimension> q3_<dimension>_<RollNo>.png
    
    
