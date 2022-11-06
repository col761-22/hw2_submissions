#pragma once

#include <vector>
#include <set>
#include <initializer_list>
#include <cassert>
#include <memory>
#include <iostream>
#include <algorithm>

class Itemset
{
    std::vector<int> elems; // First (size-1) elements
    int lastElem; // Last element
    size_t size;

public:
    // Default constructor
    Itemset();

    // Initialise with size of itemset
    Itemset( size_t length );

    // Initialise with list. Examples: Itemset I{1,2,3,4}; Itemset I = {1,2,3,4};
    Itemset( std::initializer_list<int> init );

    // Copy constructor
    Itemset( const Itemset& other );

    // Copy assignment
    const Itemset& operator=( const Itemset& other );

    // Assignment with initializer list. Example: Itemset I; I = {1,2,3,4};
    const Itemset& operator=( std::initializer_list<int> I );

    // Comparison operators
    bool operator==( const Itemset& rhs ) const;

    bool operator!=( const Itemset& rhs ) const;

    bool operator<( const Itemset& rhs ) const;

    bool operator>( const Itemset& rhs ) const;

    bool operator >=( const Itemset& rhs ) const;

    bool operator <=( const Itemset& rhs ) const;

    // Accessor functions. Note that begin and end are iterators to elems
    std::vector<int>::const_iterator getBegin() const;
    std::vector<int>::const_iterator getEnd() const;
    std::vector<int>::iterator getBegin();
    std::vector<int>::iterator getEnd();
    int getLastElem() const;
    size_t getSize() const;

    // Append an element
    void append( int x );

    // Set last element
    void setLastElem( int x );
};

std::ostream& operator<<( std::ostream& os, const Itemset& I );

// Join two itemsets. This method assumes that areJoinable(I1,I2)==true.
// Returns a pointer to a new Itemset which is the join of I1 and I2.
std::unique_ptr<Itemset> join( const Itemset& I1, const Itemset& I2 );

// Returns true iff two itemsets are joinable. For that, 
// 1. I1.elems == I2.elems
// 2. I1.lastElem < I2.lastElem
bool areJoinable( const Itemset& I1, const Itemset& I2 );

// Returns true if itemset I is a subset of transaction T.
bool isSubsetOf( const Itemset& I, const std::set<int>& T );

// Candidate Generation Function
std::vector<std::unique_ptr <Itemset>>* candidate_generation( std::vector<std::unique_ptr<Itemset>>& frequent_itemsets );

bool operator==( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs );
bool operator!=( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs );
bool operator<( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs );
bool operator>( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs );
bool operator<=( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs );
bool operator>=( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs );