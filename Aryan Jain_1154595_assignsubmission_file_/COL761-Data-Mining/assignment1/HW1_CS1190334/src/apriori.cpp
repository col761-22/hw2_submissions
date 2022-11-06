#include "itemset.hpp"
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <signal.h>

std::set<std::unique_ptr<Itemset>> Fs;
std::string outFileName;

void sig_handler( int signum )
{
    std::ofstream outfile;
    outfile.open( outFileName, std::ios::trunc );
    for ( auto& ptr : Fs )
    {
        ptr->append( ptr->getLastElem() );
        std::sort( ptr->getBegin(), ptr->getEnd(), []( const int& item1, const int& item2 ) {
            return std::to_string( item1 ) < std::to_string( item2 );
            } );
        for ( std::vector<int>::const_iterator itr = ptr->getBegin(); itr != ptr->getEnd(); itr++ )
        {
            outfile << *itr << " ";
        }
        outfile << "\n";
    }
}

std::vector<std::unique_ptr<Itemset>>* init_pass( std::string filename, int sup, int n )
{
    std::ifstream f( filename );
    std::string l;
    int a;
    std::map<int, int> items;
    std::vector<std::unique_ptr<Itemset>>* C = new std::vector<std::unique_ptr<Itemset>>();

    while ( std::getline( f, l ) )
    {

        std::stringstream ss( l );
        std::string a;
        while ( std::getline( ss, a, ' ' ) )
        {
            if ( items.find( std::stoi( a ) ) == items.end() )
            {
                items.emplace( std::stoi( a ), 1 );
            }
            else
            {
                items.at( std::stoi( a ) ) += 1;
            }

        }
        n++;
    }


    for ( auto itr : items )
    {
        if ( ( itr.second / float( n ) ) >= ( sup / 100.00 ) )
        {
            C->emplace_back( std::unique_ptr<Itemset>( new Itemset{ itr.first } ) );
        }
    }

    f.close();

    return C;
}


int main( int argc, char* argv[] )
{
    signal( SIGALRM, sig_handler ); // Register signal handler
    alarm( 3420 );
    std::string filename = argv[1];
    int sup = std::stoi( argv[2] );
    outFileName = argv[3];
    int n{ 0 };
    bool init = true; // needed for calculating n

    std::vector<std::unique_ptr<Itemset>>* C = init_pass( filename, sup, n );
    std::vector<std::unique_ptr<Itemset>> F;
    std::vector<int> freq( C->size(), 0 );

    while ( C->size() != 0 )
    {
        freq.resize( C->size() );
        F.clear();
        for ( int i = 0; i < C->size(); i++ )
        {
            freq[i] = 0;
        }

        // finding the frequency of the itemsets
        std::ifstream f( filename );
        std::string l;
        std::stringstream ss;
        std::string item;
        std::set<int> T;
        while ( getline( f, l ) )
        {
            if ( init ) n++;
            T.clear();
            ss.clear();
            ss.str( l );

            while ( std::getline( ss, item, ' ' ) )
            {
                T.insert( std::stoi( item ) );
            }


            for ( int i = 0; i < C->size(); i++ )
            {
                if ( isSubsetOf( *( ( *C )[i] ), T ) )
                {
                    freq[i]++;
                }
            }
        }
        f.close();

        // filtering out the itemsets with support less than the threshold
        for ( int i = 0; i < C->size(); i++ )
        {
            if ( ( freq[i] / float( n ) ) >= ( sup / 100.0 ) ) // freq[i] / n >= sup. TODO check Piazza. Done.
            {
                F.push_back( std::move( ( *C )[i] ) );
            }
        }

        delete C;
        C = candidate_generation( F );
        for ( int i = 0; i < F.size(); i++ )
        {
            Fs.insert( std::move( F[i] ) );
        }
        init = false;
    }
    delete C;
    std::ofstream outfile;
    outfile.open( outFileName, std::ios::trunc );
    for ( auto& ptr : Fs )
    {
        ptr->append( ptr->getLastElem() );
        std::sort( ptr->getBegin(), ptr->getEnd(), []( const int& item1, const int& item2 ) {
            return std::to_string( item1 ) < std::to_string( item2 );
            } );
        for ( std::vector<int>::const_iterator itr = ptr->getBegin(); itr != ptr->getEnd(); itr++ )
        {
            outfile << *itr << " ";
        }
        outfile << "\n";
    }
}