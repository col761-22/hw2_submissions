#!/usr/bin/bash

# upload on moodle separately, not in zip file

# Expected output after running of this script :-
# - Clone github repository.
# - Change directory to your github repo's directory.
# - Unzip the correct HW2 zipfile HW2_<RollNo>.zip in the directory.
# - Load HPC modules required by your script for compiling the source
#   by executing bash commands to load all the required HPC modules.

# Run as: source install.sh (to make use of HPC module load alias).

git clone https://github.com/guptaaniket261/Data-Mining-Assignments.git
# git clone https://github.com/guptaaniket261/Data-Mining-Assignments-Dev.git
cd Data-Mining-Assignments
unzip HW2_CS1190344.zip
module load compiler/python/3.6.0/ucs4/gnu/447
module load pythonpackages/3.6.0/matplotlib/3.0.2/gnu 
module load apps/pytorch/1.6.0/gpu/anaconda3
module load compiler/gcc/11.2.0
