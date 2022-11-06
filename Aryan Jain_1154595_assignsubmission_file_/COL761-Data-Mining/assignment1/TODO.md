1. class Itemset:
    elems = std::vector<int> of size (k-1)
    lastElem = kth element

    Methods:
        1. join(I1,I2) -> I3
        2. areJoinable(I1,I2) -> true/false
        3. isSubsetOf(I1, T: std::set<int>) -> true/false

2. Ci = std::vector<std::unique_ptr<Itemset>>

3. Maintain frequency array when making pass over transaction database. Use it later to filter Ci.

4. Ensure sorted order of Itemset, Ci.

- [ ] Understand submission instructions
- [x] Implement Apriori
    - [x] Core function : (datafile, supportThresh) => Output frequent itemsets
    - [x] Implement class Itemset
    - [x] Implement candidateGen method
    - [x] Main function
- [x] Implement FP-Tree
    - [x] Core function : (datafile, supportThresh) => Output frequent itemsets
    - [x] Main function
    - [x] TreeGrowth
    - [x] TreeConstruction
    - [x] TreeNode 
- [ ] README.txt
- [ ] <RollNo>.sh
- [ ] compile.sh
- [ ] install.sh
- [ ] Plotting program
- [ ] Timing execution of fptree and apriori (do it in bash?)
- [ ] Add timeout for 1 hour. How? Cache at 55 minutes?
- [ ] Test on hpc and webdocs dataset. Write plotting program before testing, we don't have time.
- [x] support is an integer(0-100), freq*100 >= sup * n

class TreeNode:

    1. std::string label
    2. int count
    3. TreeNode* prev
    4. TreeNode* next
    5. TreeNode* parent
    6. std::vector<TreeNode*> children
    7. bool flag
    8. int temp_count

header table = std::map<std::string, TreeNode*>
