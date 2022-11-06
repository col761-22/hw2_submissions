#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class FpTreeNode
{
public:

    int label; // Item string
    int count; // Prefix count for path from node to root

    // Header Table linked list
    FpTreeNode* prev;
    FpTreeNode* next;
    FpTreeNode* tail;

    // Tree structure
    FpTreeNode* parent;
    std::vector<FpTreeNode*> children;

    // Constructors
    FpTreeNode();
    FpTreeNode( int item, FpTreeNode* parent, int count );

    // Destructor
    ~FpTreeNode();
};

// Given a file containing transactions and the minimum support threshold(as an integer value in %),
// construct an FpTree from the database and return it. Store the linked list for each item in header table.
FpTreeNode* constructFpTree( std::string transactionFile, std::unordered_map<int, FpTreeNode*>& headerTable );