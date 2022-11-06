#!/bin/bash
module purge
module load compiler/gcc/11.2.0
module load compiler/python/3.6.0/ucs4/gnu/447
module load pythonpackages/3.6.0/matplotlib/3.0.2/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/scikit-learn/0.18.1/gnu
git clone https://github.com/battaladivyateja/data_mining_assignments_bdk.git
cd data_mining_assignments_bdk
unzip HW2_mcs212128.zip
cd HW2_mcs212128