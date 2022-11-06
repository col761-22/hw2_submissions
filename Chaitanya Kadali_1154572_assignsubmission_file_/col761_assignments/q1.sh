#!/bin/bash
g++ -O3 q1.cpp -o q1
./q1 $1
echo "Completed building all the graphs"
python q1.py q1_CS5190435
rm -r Dump/
echo "Completed plotting the graph. Please take a look at q1_CS5190435.png"
