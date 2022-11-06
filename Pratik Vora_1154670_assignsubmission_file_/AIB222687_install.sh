#!/usr/bin/bash

git clone https://github.com/IITD-Assignments/Team-Persistence.git

if [ "${?}" -ne "0" ]; then
    echo "Failed to clone repository. Aborting..."
    exit 1
fi

echo "Loading modules..."
module load compiler/gcc/9.1.0
module load compiler/intel/2019u5/intelpython3
