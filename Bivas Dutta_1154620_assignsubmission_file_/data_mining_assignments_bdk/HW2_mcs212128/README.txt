#Team Members               Contribution
    1. Kriti Kaushal           33%
    2. Bivas Ranjan Dutta      33%
    3. Batalla DivyaTeja       33%

#Code Explanation

=> The folder has three sub folders
    1. Q1
        - preprocessing.py
            => This creates the files that are required to run the binaries.
            => python3 preprocessing.py fsg
                - This will generate the dataset to run fsg
            => python3 preprocessing.py gSpan
                - This will generate the dataset to run gSpan
            => python3 preprocessing.py gaston
                - This will generate the dataset to run gaston
        - draw.py
            => This will create the plots, to compare the algorithms.
            => To run the code, type:
                    python3 draw.py
    2. Q2
	    - preprocessing.py
            => Coverts the given format to desired format
        -index_build.py
            => It creates the index and for every query file gives the graph ids for which are subgraph isomorphic to given query.
            => python3 index_build.py <input_graph> <queries>
                - This code will run for input_graph. And for each query of queries;
                This will print the graph ids which are subgraph isomorphic to it.
        -demo.cpp
            => To run the vf2 algorithm.
            => g++ -Iinclude/ demo.cpp
                => To compile the code
            => a.out <query> <graphfile> 
                => this will print "done" if query is subgraph isomorphic to graphfile else it will print "not".


    3. Q3
        -draw.py
            => This generates the elbow plot for given dataset.
            => python3 draw.py <input_file> <output_file.png>
                - This will create the elbow plot for input_file as save the figure as output_file.png
        -elbow_plot.sh
            => This script will call the draw.py file to generate the plot.
            => ./elbow_plot.sh <dataset> <dim> q3_<dim>_<roolno>.png
                - This will generate the plot for the dataset which is a dimension of <dim> and save the plot as q3_<dim>_<roolno>.png
            
