#####################################################################
#########  COL 761 - ASSIGNMENT 2  ########
#####################################################################
#####################  TEAM MEMBERS  ################################
APAR SINGHAL(2021MCS2124) : 34%
DEVESH KUMAR(2021MCS2130) : 33%
SACHIN SINGH(2021MCS2149) : 33%
Note: All members have equally contributed for assignment development
#####################################################################
#########################  FILES BUNDLED  ###########################

MCS212124_install.sh - This shell script will clone the zip file of assignment 1 containing everything.

plot.sh - This is shell script to run Question 1

compile.sh - This file compiles all the cpp files for Q2
index.sh - builds index structures for Q2
query.sh - performs query processing to generate output for Q2
q3.cpp - builds index structures for Q2
q3.hpp - header file for Q2
q3_query.cpp - performs query processing to generate output for Q2

elbow_plot.sh - This is shell script to run Question 3

plot.py - It contains code for making plots depicting comparison between fsg, gspan and gaston algorithms according to parameter time taken with respect to different threshold in this list [5,10,25,50,90].

q3_MCS212124_kmeans.py - Code for Kmeans Question 3

MCS212124_Report.pdf - report for Assignment Submitted

#####################################################################
########################  HOW TO EXECUTE  ###########################
1. First run install.sh that will clone the zip file of assignment 1 containing everything.
2. For running the Question Number
	1st -> sh plot.sh arg1 arg2  
		where arg1 is input filename, 
		arg2 is output file.png for the plot. 
	2nd -> sh compile.sh  
	       sh index.sh <graph_dataset>
	       sh query.sh
	3rd -> sh elbow_plot.sh arg1 arg2 arg3
		where arg1 is input filename, 
		arg2 is Number of dimensions,
		arg3 is output file.png for the plot. 
		
#####################################################################








