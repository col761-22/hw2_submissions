#include "fptree.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <iostream>
#include <stack>
#include <set>
#include <bitset>
#include <unordered_set>
#include <memory>
#include <unistd.h>
#include <signal.h>

// Custom comparator for sets of strings (i.e., itemsets)
auto cmp = []( const std::unique_ptr<std::unordered_set<int>>& I1, const std::unique_ptr<std::unordered_set<int>>& I2 ) {
    std::unordered_set<int>& items1 = *I1;
    std::unordered_set<int>& items2 = *I2;
    std::unordered_set<int>::const_iterator iter1, iter2;
    for ( iter1 = items1.begin(), iter2 = items2.begin(); ( iter1 != items1.end() ) and ( iter2 != items2.end() ); iter1++, iter2++ )
    {
        if ( ( *iter1 ) < ( *iter2 ) ) return true;
        else if ( ( *iter1 ) > ( *iter2 ) ) return false;
    }
    if ( iter1 == items1.end() )
    {
        if ( iter2 == items2.end() )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
};

// Global variables for FpTree Growth
int numTransactions, support;
std::unordered_map<int, int> freqs;
std::unordered_set<std::unique_ptr<std::unordered_set<int>>> frequentItemsets;
std::string outFile;

void sig_handler( int signum )
{
    std::ofstream outfile;
    outfile.open( outFile, std::ios::trunc );
    for ( auto& itemsetPtr : frequentItemsets )
    {
        std::unordered_set<int>& itemset = *itemsetPtr;
        for ( auto iter = itemset.begin(); iter != itemset.end(); iter++ )
        {
            if ( iter == std::prev( itemset.end() ) ) outfile << *iter;
            else outfile << *iter << " ";
        }
        outfile << std::endl;
    }
}

FpTreeNode::FpTreeNode() : label{ 0 }, count{ -1 },
prev{ nullptr }, next{ nullptr }, tail{ nullptr }, parent{ nullptr }
{
};

FpTreeNode::FpTreeNode( int item, FpTreeNode* parPtr, int cnt ) : label{ item }, count{ cnt },
prev{ nullptr }, next{ nullptr }, tail{ nullptr }, parent{ parPtr }
{
};

FpTreeNode::~FpTreeNode()
{
    prev = next = tail = parent = nullptr;
    for ( auto& p : children )
    {
        delete p;
    }
}

// Given a file containing transactions and the minimum support threshold(as an integer value in %),
// construct an FpTree from the database and return it. Store the linked list for each item in header table.
FpTreeNode* constructFpTree( std::string transactionFile, std::unordered_map<int, FpTreeNode*>& headerTable )
{
    // ################################PHASE I#####################################
    // First find set of items and their frequencies

    std::ifstream db( transactionFile );
    std::string line;
    int n = 0; // number of transactions
    while ( getline( db, line ) )
    {
        n++;
        std::stringstream ss( line );
        // int i;
        std::string i;
        int a;
        while ( std::getline( ss, i, ' ' ) )
        {
            a = std::stoi( i );
            if ( freqs.find( a ) == freqs.end() )
            {
                freqs.insert( { a,1 } );
            }
            else
            {
                freqs.at( a ) += 1;
            }
        }
    }
    numTransactions = n;

    // Only consider frequent items
    for ( auto iter = freqs.begin(); iter != freqs.end(); )
    {
        if (
            ( ( *iter ).second / float( n ) ) < ( support / 100.0 ) // TODO check condition
            ) iter = freqs.erase( iter );
        else iter++;
    }

    // ##############################PHASE II###########################
    // FpTree Construction
    FpTreeNode* root = new FpTreeNode();
    db.clear();
    db.seekg( 0, std::ios::beg );

    while ( getline( db, line ) )
    {
        std::vector<int> T;
        std::stringstream ss( line );
        // int i;
        std::string i;
        int a;
        while ( std::getline( ss, i, ' ' ) )
        {
            a = std::stoi( i );
            if ( freqs.find( a ) != freqs.end() )
            {
                T.push_back( a ); // Only consider frequent items
            }
        }

        // Sort in descending order of support
        std::sort( T.begin(), T.end(), []( const int& i1, const int& i2 ) {
            return ( freqs.at( i1 ) > freqs.at( i2 ) );
            } );

        // Add transaction to tree
        FpTreeNode* current = root;
        for ( auto item : T )
        {
            // Check if item is one of the children of current node
            bool found = false;
            FpTreeNode* foundPtr = nullptr;
            for ( FpTreeNode* child : current->children )
            {
                if ( child->label == item )
                {
                    found = true;
                    foundPtr = child;
                    break;
                }
            }
            if ( found )
            {
                foundPtr->count++;
                current = foundPtr;
            }
            else
            {
                // Create a new child node for this item
                FpTreeNode* newChild = new FpTreeNode( item, current, 1 );
                current->children.push_back( newChild );
                // Add this new node to header table
                auto pos = headerTable.find( item );
                if ( pos == headerTable.end() )
                {
                    newChild->tail = newChild;
                    headerTable.insert( { item, newChild } );
                }
                else
                {
                    FpTreeNode* node = ( *pos ).second;
                    node->tail->next = newChild;
                    newChild->prev = node->tail;
                    node->tail = newChild;


                }
                current = newChild;
            }
        }
    }
    return root;
}

std::unordered_set<std::unique_ptr<std::unordered_set<int>>>* generate_combinations( std::set<int> items, std::vector<int> suffix )
{
    // Generate all subsets of items. Concat them with suffix to generate sets, and return a set of pointers to
    // those sets.

    std::unordered_set<std::unique_ptr<std::unordered_set<int>>>* powerSet = new std::unordered_set<std::unique_ptr<std::unordered_set<int>>>();
    int n = items.size();
    int k = 1;
    while ( k < ( 1 << n ) )
    {
        int i = 0;
        std::unique_ptr<std::unordered_set<int>> subset( new std::unordered_set<int>( suffix.begin(), suffix.end() ) );
        for ( auto itr = items.begin(); itr != items.end(); itr++, i++ )
        {
            if ( k & ( 1 << i ) )
            {
                subset->insert( *itr );
            }
        }

        powerSet->insert( std::move( subset ) );
        k++;
    }

    return powerSet;
}

// Given FpTree, grow the existing suffix pattern.
void fpGrowth( FpTreeNode* root, std::vector<int> suffix,
    std::unordered_map<int, FpTreeNode*>& headerTable,
    std::unordered_set<std::unique_ptr<std::unordered_set<int>>>* frequentPatterns )
{

    FpTreeNode* curr = root;

    if ( curr->children.size() == 0 )
    {
        return;
    }

    // TODO clean up if not using
    while ( curr->children.size() == 1 )
    {
        curr = curr->children[0];
    }

    if ( curr->children.size() == 0 )
    {
        std::set<int> items;
        while ( curr->parent != nullptr )
        {
            if ( ( curr->count / float( numTransactions ) ) < ( support / 100.0 ) )
            {
                curr = curr->parent;
            }
            else
            {
                items.insert( curr->label );
                curr = curr->parent;
            }
        }
        std::unordered_set<std::unique_ptr<std::unordered_set<int>>>* itemsets = generate_combinations( items, suffix );
        for ( std::unordered_set<std::unique_ptr<std::unordered_set<int>>>::iterator iter = itemsets->begin(); iter != itemsets->end(); iter++ )
        {
            std::unique_ptr<std::unordered_set<int>> ptr( new std::unordered_set<int>( *( *iter ) ) );
            frequentPatterns->insert( std::move( ptr ) );
        }

        delete itemsets;
        return;
    }

    for ( auto& head : headerTable )
    {
        FpTreeNode* curr = head.second;
        int count = 0;
        while ( curr != nullptr )
        {
            count += curr->count;
            curr = curr->next;
        }
        if ( ( count / float( numTransactions ) ) < ( support / 100.0 ) ) // TODO check condition
        {
            continue;
        }
        std::unique_ptr<std::unordered_set<int>> itemset( new std::unordered_set<int>{ head.first } );
        for ( auto item : suffix )
        {
            itemset->insert( item );
        }
        frequentPatterns->insert( std::move( itemset ) );
        // Create conditional tree
        FpTreeNode* condTree = new FpTreeNode();
        // Create new header table
        std::unordered_map<int, FpTreeNode*> newHeaderTable;
        curr = head.second;
        while ( curr != nullptr )
        {
            std::vector<int> transaction;
            FpTreeNode* par = curr->parent;
            while ( par->parent != nullptr )
            {
                transaction.push_back( par->label );
                par = par->parent;
            }
            // Sort in descending order of support
            std::sort( transaction.begin(), transaction.end(), []( const int& i1, const int& i2 ) {
                return ( freqs.at( i1 ) > freqs.at( i2 ) );
                } );

            // Insert transaction into conditional tree

            // Add transaction to tree
            FpTreeNode* current = condTree;
            for ( auto item : transaction )
            {
                // Check if item is one of the children of current node
                bool found = false;
                FpTreeNode* foundPtr = nullptr;
                for ( FpTreeNode* child : current->children )
                {
                    if ( child->label == item )
                    {
                        found = true;
                        foundPtr = child;
                        break;
                    }
                }
                if ( found )
                {
                    foundPtr->count += curr->count;
                    current = foundPtr;
                }
                else
                {
                    // Create a new child node for this item
                    FpTreeNode* newChild = new FpTreeNode( item, current, curr->count );
                    current->children.push_back( newChild );
                    // Add this new node to header table
                    auto pos = newHeaderTable.find( item );
                    if ( pos == newHeaderTable.end() )
                    {
                        newChild->tail = newChild;
                        newHeaderTable.insert( { item, newChild } );
                    }
                    else
                    {
                        FpTreeNode* node = ( *pos ).second;
                        node->tail->next = newChild;
                        newChild->prev = node->tail;
                        node->tail = newChild;
                    }
                    current = newChild;
                }
            }
            curr = curr->next;
        }
        std::vector<int> newSuffix( suffix.size() + 1 );
        newSuffix[0] = head.first;
        for ( int j = 0; j < suffix.size(); j++ ) newSuffix[j + 1] = suffix[j];
        fpGrowth(
            condTree,
            newSuffix,
            newHeaderTable,
            frequentPatterns
        );

        delete condTree;
    }
}

int main( int argc, char* argv[] )
{
    signal( SIGALRM, sig_handler ); // Register signal handler
    alarm( 3420 );
    std::string inFile = argv[1];
    support = std::stoi( argv[2] );
    outFile = argv[3];
    std::unordered_map<int, FpTreeNode*> headerTable;

    FpTreeNode* root = constructFpTree(
        inFile,
        headerTable
    );

    std::vector<int> suffix;
    fpGrowth(
        root,
        suffix,
        headerTable,
        &frequentItemsets
    );
    delete root;

    std::ofstream outfile( outFile, std::ios::trunc );
    for ( auto& itemsetPtr : frequentItemsets )
    {
        std::unordered_set<int>& itemset = *itemsetPtr;
        std::vector<int> vecItemset( itemset.begin(), itemset.end() );
        std::sort( vecItemset.begin(), vecItemset.end(), []( const int& item1, const int& item2 ) {
            return std::to_string( item1 ) < std::to_string( item2 );
            } );
        for ( auto iter = vecItemset.begin(); iter != vecItemset.end(); iter++ )
        {
            outfile << *iter << " ";
        }
        outfile << std::endl;
    }
}