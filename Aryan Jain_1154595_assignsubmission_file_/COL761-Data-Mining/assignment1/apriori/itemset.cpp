#include "itemset.hpp"

// Default constructor
Itemset::Itemset() : size( 0 ) {};

// Initialise with size of itemset
Itemset::Itemset( size_t length ) : size{ length }
{
    if ( length > 0 ) elems.resize( length - 1 );
}

// Initialise with list. Examples: Itemset I{1,2,3,4}; Itemset I = {1,2,3,4};
Itemset::Itemset( std::initializer_list<int> init ) : elems( init.begin(), std::prev( init.end() ) ),
lastElem{ *std::prev( init.end() ) }, size{ init.size() }
{
};

// Copy constructor
Itemset::Itemset( const Itemset& other ) : elems( other.getSize() - 1, 0 ), size{ other.getSize() }
{
    assert( other.getSize() > 0 ); // TODO better way to ensure size>0
    std::copy( other.getBegin(), other.getEnd(), elems.begin() );
    lastElem = other.getLastElem();
};

// Copy assignment
const Itemset& Itemset::operator=( const Itemset& other )
{
    size = other.getSize();
    assert( size > 0 );
    elems.resize( size - 1 );
    std::copy( other.getBegin(), other.getEnd(), elems.begin() );
    lastElem = other.getLastElem();
    return *this;
};

// Assignment with initializer list. Example: Itemset I; I = {1,2,3,4};
const Itemset& Itemset::operator=( std::initializer_list<int> I )
{
    size = I.size();
    assert( size > 0 );
    elems.resize( size - 1 );
    std::copy( I.begin(), std::prev( I.end() ), elems.begin() );
    lastElem = *std::prev( I.end() );
    return *this;
};

// Comparison operators
bool Itemset::operator==( const Itemset& rhs ) const
{
    if ( size != rhs.getSize() ) return false;
    if ( lastElem != rhs.getLastElem() ) return false;
    for ( auto i1 = elems.begin(), i2 = rhs.getBegin(); i1 != elems.end(); i1++, i2++ )
    {
        if ( ( *i1 ) != ( *i2 ) ) return false;
    }
    return true;
};

bool Itemset::operator!=( const Itemset& rhs ) const
{
    return !( operator==( rhs ) );
};

bool Itemset::operator<( const Itemset& rhs ) const
{
    std::vector<int>::const_iterator itr1 = elems.begin(), itr2 = rhs.getBegin();
    for ( ; ( itr1 != elems.end() ) and ( itr2 != rhs.getEnd() ); itr1++, itr2++ )
    {
        if ( ( *itr1 ) < ( *itr2 ) ) return true;
        else if ( ( *itr2 ) < ( *itr1 ) ) return false;
    }
    if ( itr1 == elems.end() )
    {
        if ( itr2 == rhs.getEnd() )
        {
            if ( lastElem < rhs.getLastElem() ) return true;
            else return false;
        }
        else
        {
            if ( lastElem < *itr2 ) return true;
            else if ( lastElem > *itr2 ) return false;
            else return true;
        }
    }
    else
    {
        if ( rhs.getLastElem() < *itr1 ) return false;
        else if ( rhs.getLastElem() > *itr1 ) return true;
        else return false;
    }
}

bool Itemset::operator>( const Itemset& rhs ) const
{
    return !( operator==( rhs ) or operator<( rhs ) );
}

bool Itemset::operator<=( const Itemset& rhs ) const
{
    return ( operator<( rhs ) or operator==( rhs ) );
}

bool Itemset::operator>=( const Itemset& rhs ) const
{
    return ( operator>( rhs ) or operator==( rhs ) );
}

// Accessor functions. Note that begin and end are iterators to elems
std::vector<int>::const_iterator Itemset::getBegin() const { return elems.begin(); };
std::vector<int>::const_iterator Itemset::getEnd() const { return elems.end(); };
std::vector<int>::iterator Itemset::getBegin() { return elems.begin(); };
std::vector<int>::iterator Itemset::getEnd() { return elems.end(); };
int Itemset::getLastElem() const { return lastElem; }
size_t Itemset::getSize() const { return size; }

// Append an element
void Itemset::append( int x )
{
    if ( size > 0 )
    {
        elems.push_back( lastElem );
        lastElem = x;
        size++;
    }
    else
    {
        lastElem = x;
        size++;
    }
}

// Set last element
void Itemset::setLastElem( int x ) { lastElem = x; };

std::ostream& operator<<( std::ostream& os, const Itemset& I )
{
    std::cout << "=============ITEMSET============\nSize: " << I.getSize() << "\nElements: ";
    std::for_each( I.getBegin(), I.getEnd(), []( int x ) { std::cout << x << " "; } );
    std::cout << I.getLastElem() << std::endl;
    std::cout << "================================" << std::endl;
    return os;
};

// Join two itemsets. This method assumes that areJoinable(I1,I2)==true.
// Returns a pointer to a new Itemset which is the join of I1 and I2.
std::unique_ptr<Itemset> join( const Itemset& I1, const Itemset& I2 )
{
    std::unique_ptr<Itemset> I3( new Itemset( I1 ) ); // I3 is a copy of I1
    I3->append( I2.getLastElem() );
    return I3;
}

// Returns true iff two itemsets are joinable. For that, 
// 1. I1.elems == I2.elems
// 2. I1.lastElem < I2.lastElem
bool areJoinable( const Itemset& I1, const Itemset& I2 )
{
    if ( I1.getSize() != I2.getSize() ) return false;
    if ( I1.getLastElem() >= I2.getLastElem() ) return false;
    for ( auto i1 = I1.getBegin(), i2 = I2.getBegin(); i1 != I1.getEnd(); i1++, i2++ )
    {
        if ( ( *i1 ) != ( *i2 ) ) return false;
    }
    return true;
}

// Returns true if itemset I is a subset of transaction T.
bool isSubsetOf( const Itemset& I, const std::set<int>& T )
{
    if (I.getSize()==0) return true;
    if (I.getSize()==1) return (T.find(I.getLastElem()) != T.end());
    auto pos2 = T.find(*I.getBegin());
    if (pos2 == T.end()) return false;
    auto pos1 = I.getBegin();
    while ( (pos1 != I.getEnd()) && (pos2 != T.end()) )
    {
        if ( (*pos1) == (*pos2) )
        {
            pos1++;
            pos2++;
        }else {
            while( (pos2 != T.end()) && (*pos2 != *pos1)) pos2++;
        }
    }
    if (pos1 != I.getEnd()) return false;
    if (pos2 == T.end()) return false;
    return (std::find(
        pos2, T.end(), I.getLastElem()
    ) != T.end());
}

bool operator==( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs )
{
    return ( *lhs ) == ( *rhs );
}

bool operator!=( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs )
{
    return !( ( *lhs ) == ( *rhs ) );
}

bool operator<( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs )
{
    return ( ( *lhs ) < ( *rhs ) );
}

bool operator>( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs )
{
    return ( ( *lhs ) > ( *rhs ) );
}

bool operator<=( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs )
{
    return ( ( *lhs ) <= ( *rhs ) );
}

bool operator>=( const std::unique_ptr<Itemset>& lhs, const std::unique_ptr<Itemset>& rhs )
{
    return ( ( *lhs ) >= ( *rhs ) );
}