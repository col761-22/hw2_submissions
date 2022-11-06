#!/usr/bin/bash

# sh elbow_plot.sh <dataset> <dimension> q3_<dimension>_<RollNo>.png

if [ $# -ne 3 ]
then
    echo "Usage: sh elbow_plot.sh <dataset> <dimension> <plot_name>"
    # example usage: sh elbow_plot.sh CS1190344_generated_dataset_3D.dat 3 q3_3_CS1190344.png
    exit 1
else
    python3 generate_elbow_plot.py $1 $2 $3
fi
