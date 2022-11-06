#!/usr/bin/bash
g++ -O3 -o apriori src/apriori.cpp src/candidate_generation.cpp src/itemset.cpp -I src/
g++ -O3 -o fptree src/fptree.cpp -I src/
