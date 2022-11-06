#!/bin/bash

#usage: /usr/bin/time -v ./index.sh 167.txt_graph

threshold=0.4

g++ data_format_converter.cpp -O3 -o data_format_converter
./data_format_converter $1

../gSpan6/gSpan-64 -f "data_for_gspan.txt" -s $threshold -o -i

g++ index.cpp -O3 -o index
./index "data_for_gspan.txt.fp" 100000 $1 # m is passed as 100,000 but is ignored as we select all graphs at 0.4 threshold

g++ gspan_data_format_to_vf2.cpp -O3 -o gspan_data_format_to_vf2
./gspan_data_format_to_vf2 --type graphs --input data_for_gspan.txt
./gspan_data_format_to_vf2 --type patterns --input data_for_gspan.txt.fp --selected_patterns selected_patterns.txt

rm data_format_converter
rm index
rm gspan_data_format_to_vf2