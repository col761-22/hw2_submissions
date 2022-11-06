## Frequent Itemset Mining
#### Github Link :  
https://github.com/Ramneet-Singh/COL761-Data-Mining.git

#### Team Details

| Ramneet Singh| Aryan Jain    | Anjali Sharma  |
| ------------ | ------------- | -------------- |
| 2019CS50445  | 2019CS10334   | 2019CS50422    |
|    33%       |      33%      |    33%	        |

Individual contributions :
Aryan : apriori.cpp, fptree.cpp, bash scripts
Ramneet : itemset.cpp, itemset.hpp, fptree.cpp, bash scripts
Anjali : candidate_generation.cpp, fptree.cpp, bash scripts


#### Submitted Files
* src
	* itemset.hpp : defined itemset class which has
		* vector of items 
		* last element
				
	* itemset.cpp : defined operators and following functions
	 	* join (itemset I1 , itemset I2)
	 	* areJoinable (itemset I1, itemset I2)
	 	* isSubsetOf (itemset I, Transactions)
						
	* candidate_generation.cpp : takes input as vector of frequent itemsets os size k and returns the possible k+1 size candidates.
	* apriori.cpp : implementation of Apriori algorithm
	* fptree.hpp : header file for fptree, defines fptree class
	* fptree.cpp : implementation of fptree algorithm ( Tree construction and Tree growth)
* CS1190334.sh : Executing  “./<RollNo>.sh -apriori <dataset_name> X <outputFilename>” generates an output file containing the frequent itemset at >=X% support threshold with the Apriori algorithm.
 				   Similarly “./<RollNo>.sh -fptree <dataset_name> X <output filename>” uses FP- Tree.
* compile.sh : compiles the code for all the files.
* install.sh : clones git repo and unzips the required zip file , loads hpc modules.
* plotting.py: This script will generate plots for the minimum support values as [5,10,25,50,90] for Apriori and FPtree.

#### Apriori vs FpTree  ( Explanation for Q3)

- Apriori algorithm requires k database scans to generate frequent itemsets upto length k. 
On the other hand, FpTree requires only 2 database scans. Large datasets can not be loaded 
into the main memory and data access to file storage is approximately 1000 times slower,
this makes database scans an extremely costly ordeal. Hence, this helps FpTree be much more 
time efficient than Apriori.

- The inherent cost of candidate generation in Apriori is extremely high. For eg. generating a
frequent pattern of size k, Apriori generates approximately 2^k candidates. On the other hand,
FpTree has no candidate generation step, and grows frequent patterns in a depth first manner,
employing early pruning of paths that would not lead to frequent itemsets using the ordering 
of the single itemsets.

- FpTree is much more scalable compared to Apriori as the minimum support threshold is decreased.
Both the algorithms see a drastic increase in runtime as the minimum support threshold decreases,
however, the increasing amount of database scans in Apriori make it increasingly slower compared to
FpTree.



