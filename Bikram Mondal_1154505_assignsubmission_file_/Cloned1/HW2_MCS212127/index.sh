#!/bin/bash

g++ -Iinclude/ -std=c++11 vf2check.cpp
python3 buildIndex.py $1