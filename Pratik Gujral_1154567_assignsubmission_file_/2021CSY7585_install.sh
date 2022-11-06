#!/bin/bash
git clone https://github.com/pratikgujral/IITD-COL761-DataMining
cd IITD-COL761-DataMining
unzip HW2_2021CSY7585.zip
cd HW2_2021CSY7585
module load compiler/gcc/7.1.0/compilervars
module load compiler/intel/2019u5/intelpython3
export INST_DIR=$HOME/pythonpackages/python3/scipy
mkdir -p $INST_DIR
pip install matplotlib --prefix=$INST_DIR
export PYTHONPATH=$INST_DIR/python3.6/site-packages:$PYTHONPATH